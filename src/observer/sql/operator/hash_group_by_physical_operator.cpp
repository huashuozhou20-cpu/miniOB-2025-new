/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2024/05/30.
//

#include "common/log/log.h"
#include "sql/operator/hash_group_by_physical_operator.h"
#include <unordered_map>

using namespace std;
using namespace common;
// build a stable composite key for a tuple of values
static inline string build_group_key(const Tuple &tuple)
{
  string key;
  const int n = tuple.cell_num();
  key.reserve(n * 8);
  for (int i = 0; i < n; i++) {
    Value v;
    if (tuple.cell_at(i, v) != RC::SUCCESS) {
      continue;
    }
    key.push_back('\x1e'); // unit separator
    key += std::to_string(static_cast<int>(v.attr_type()));
    key.push_back(':');
    key += v.to_string();
  }
  return key;
}


HashGroupByPhysicalOperator::HashGroupByPhysicalOperator(
    vector<unique_ptr<Expression>> &&group_by_exprs, vector<Expression *> &&expressions)
    : GroupByPhysicalOperator(std::move(expressions)), group_by_exprs_(std::move(group_by_exprs))
{
}

RC HashGroupByPhysicalOperator::open(Trx *trx)
{
  LOG_INFO("open hash group by operator");
  ASSERT(children_.size() == 1, "group by operator only support one child, but got %d", children_.size());

  PhysicalOperator &child = *children_[0];
  RC                rc    = child.open(trx);
  if (OB_FAIL(rc)) {
    LOG_INFO("failed to open child operator. rc=%s", strrc(rc));
    return rc;
  }

  have_value = false;
  is_null = false;
  return rc;
}

RC HashGroupByPhysicalOperator::next()
{
  if(!have_value){
    PhysicalOperator &child = *children_[0];
    RC                rc    = RC::SUCCESS;
    ExpressionTuple<Expression *> group_value_expression_tuple(value_expressions_);

    ValueListTuple group_by_evaluated_tuple;

    while (OB_SUCC(rc = child.next())) {
      rc = collect(group_value_expression_tuple);
      if(rc != RC::SUCCESS)return rc;
    }

    if (RC::RECORD_EOF == rc) {
      rc = RC::SUCCESS;
    }

    if (OB_FAIL(rc)) {
      LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
      return rc;
    }

    // 得到最终聚合后的值
    if(groups_.size() == 0)
    {
      is_null = true;
      AggregatorList aggregator_list;
      create_aggregator_list(aggregator_list);

      groups_.emplace_back(ValueListTuple(), GroupValueType(std::move(aggregator_list), CompositeTuple()));
    }
    for (GroupType &group : groups_) {
      GroupValueType &group_value = get<1>(group);
      rc = evaluate(group_value, true);  // have_groub_by = true for HashGroupBy
      if (OB_FAIL(rc)) {
        LOG_WARN("failed to evaluate group value. rc=%s", strrc(rc));
        return rc;
      }
    }

    current_group_ = groups_.begin();
    first_emited_  = false;
    have_value = true;
  }

  return fetch_next();
}

RC HashGroupByPhysicalOperator::next(Tuple *upper_tuple)
{
  if(!have_value){
    PhysicalOperator &child = *children_[0];
    RC                rc    = RC::SUCCESS;
    ExpressionTuple<Expression *> group_value_expression_tuple(value_expressions_);

    ValueListTuple group_by_evaluated_tuple;

    while (OB_SUCC(rc = child.next(upper_tuple))) {
      rc = collect(group_value_expression_tuple);
      if(rc != RC::SUCCESS)return rc;
    }

    if (RC::RECORD_EOF == rc) {
      rc = RC::SUCCESS;
    }

    if (OB_FAIL(rc)) {
      LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
      return rc;
    }

    // 得到最终聚合后的值
    if(groups_.size() == 0)
    {
      is_null = true;
      AggregatorList aggregator_list;
      create_aggregator_list(aggregator_list);
      groups_.emplace_back(ValueListTuple(), GroupValueType(std::move(aggregator_list), CompositeTuple()));
    }
    for (GroupType &group : groups_) {
      GroupValueType &group_value = get<1>(group);
      rc = evaluate(group_value, true);  // have_groub_by = true for HashGroupBy
      if (OB_FAIL(rc)) {
        LOG_WARN("failed to evaluate group value. rc=%s", strrc(rc));
        return rc;
      }
    }

    current_group_ = groups_.begin();
    first_emited_  = false;
    have_value = true;
  }

  return fetch_next();
}

RC HashGroupByPhysicalOperator::close()
{
  children_[0]->close();
  std::vector<GroupType> temp;
  groups_.swap(temp);
  LOG_INFO("close hash group by operator");
  return RC::SUCCESS;
}

Tuple *HashGroupByPhysicalOperator::current_tuple()
{
  if (current_group_ != groups_.end()) {
    // GroupType = std::tuple<ValueListTuple, GroupValueType>
    // 第一个元素是 GROUP BY 列的值，第二个元素是聚合值
    ValueListTuple &group_by_values = get<0>(*current_group_);
    GroupValueType &group_value = get<1>(*current_group_);
    CompositeTuple &composite_value_tuple = get<1>(group_value);
    
    // 创建一个新的 CompositeTuple，包含 GROUP BY 列和聚合值
    // 使用成员变量来存储，避免返回局部变量地址
    result_tuple_ = CompositeTuple();
    
    // 添加 GROUP BY 列的值
    if (group_by_values.cell_num() > 0) {
      result_tuple_.add_tuple(make_unique<ValueListTuple>(group_by_values));
    }
    
    // 添加聚合值（composite_value_tuple 的最后一个 tuple 是聚合值）
    // 根据代码逻辑：
    // - 对于非空表且有聚合函数：composite_value_tuple 包含两个 tuple
    //   - 第一个 tuple：child_tuple_to_value（原始数据，在 find_group 中添加）
    //   - 第二个 tuple：聚合值（在 evaluate 中添加）
    // - 对于非空表但没有聚合函数：composite_value_tuple 只有一个 tuple
    //   - 第一个 tuple：child_tuple_to_value（原始数据，在 find_group 中添加）
    // - 对于空表：composite_value_tuple 只有一个 tuple
    //   - 第一个 tuple：聚合值（在 evaluate 中添加）
    // 判断方法：
    // - 检查 aggregate_expressions_ 是否为空来判断是否有聚合函数
    // - 如果 group_by_values.cell_num() > 0 且 aggregate_expressions_ 不为空，说明有聚合函数
    // - 如果 group_by_values.cell_num() > 0 且 aggregate_expressions_ 为空，说明没有聚合函数
    // - 如果 group_by_values.cell_num() == 0，说明是空表，composite_value_tuple 只有一个 tuple（聚合值）
    if (group_by_values.cell_num() > 0) {
      // 非空表，检查是否有聚合函数
      // 通过检查 aggregate_expressions_ 是否为空来判断
      if (!aggregate_expressions_.empty()) {
        // 有聚合函数，composite_value_tuple 应该有两个 tuple，使用 tuple_at(1) 获取聚合值
        // evaluate 函数会将聚合值 tuple 添加到 composite_value_tuple 的末尾
        result_tuple_.add_tuple(make_unique<ValueListTuple>(static_cast<ValueListTuple&>(composite_value_tuple.tuple_at(1))));
      }
      // 如果没有聚合函数（aggregate_expressions_ 为空），不需要添加聚合值
      // 因为查询只有 GROUP BY 列，没有聚合函数
    } else {
      // 空表，使用 tuple_at(0) 获取聚合值
      if (composite_value_tuple.get_tuple_size() > 0) {
        result_tuple_.add_tuple(make_unique<ValueListTuple>(static_cast<ValueListTuple&>(composite_value_tuple.tuple_at(0))));
      }
    }
    
    return &result_tuple_;
  }
  return nullptr;
}
RC HashGroupByPhysicalOperator::find_group(const Tuple &child_tuple, GroupType *&found_group)
{
  found_group = nullptr;

  RC rc = RC::SUCCESS;

  ExpressionTuple<unique_ptr<Expression>> group_by_expression_tuple(group_by_exprs_);
  ValueListTuple                          group_by_evaluated_tuple;
  group_by_expression_tuple.set_tuple(&child_tuple);
  rc = ValueListTuple::make(group_by_expression_tuple, group_by_evaluated_tuple);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to get values from expression tuple. rc=%s", strrc(rc));
    return rc;
  }

  // 找到对应的group（使用key索引以支持多字段稳定分组）
  static unordered_map<string, size_t> group_index; // scoped to operator instance implicitly by TU; reset on open/close
  if (groups_.empty()) {
    group_index.clear();
  }
  const string key = build_group_key(group_by_evaluated_tuple);
  auto it = group_index.find(key);
  if (it != group_index.end()) {
    found_group = &groups_[it->second];
  }

  // 如果没有找到对应的group，创建一个新的group
  if (nullptr == found_group) {
    AggregatorList aggregator_list;
    create_aggregator_list(aggregator_list);

    ValueListTuple child_tuple_to_value;
    rc = ValueListTuple::make(child_tuple, child_tuple_to_value);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to make tuple to value list. rc=%s", strrc(rc));
      return rc;
    }

    CompositeTuple composite_tuple;
    composite_tuple.add_tuple(make_unique<ValueListTuple>(std::move(child_tuple_to_value)));
    groups_.emplace_back(std::move(group_by_evaluated_tuple), 
                         GroupValueType(std::move(aggregator_list), std::move(composite_tuple)));
    found_group = &groups_.back();
    group_index.emplace(key, groups_.size() - 1);
  }

  return rc;
}

RC HashGroupByPhysicalOperator::collect(ExpressionTuple<Expression *> &group_value_expression_tuple)
{
  RC rc = RC::SUCCESS;
  Tuple *child_tuple = (*children_[0]).current_tuple();
  if (nullptr == child_tuple) {
    LOG_WARN("failed to get tuple from child operator. rc=%s", strrc(rc));
    return RC::INTERNAL;
  }

  // 找到对应的group
  GroupType *found_group = nullptr;
  rc                     = find_group(*child_tuple, found_group);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to find group. rc=%s", strrc(rc));
    return rc;
  }

  // 计算需要做聚合的值
  group_value_expression_tuple.set_tuple(child_tuple);

  // 计算聚合值
  GroupValueType &group_value = get<1>(*found_group);
  rc = aggregate(get<0>(group_value), group_value_expression_tuple);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to aggregate values. rc=%s", strrc(rc));
    return rc;
  }

  return rc;
}

RC HashGroupByPhysicalOperator::fetch_next()
{
  if (current_group_ == groups_.end()) {
    return RC::RECORD_EOF;
  }

  if (first_emited_) {
    ++current_group_;
  } else {
    first_emited_ = true;
  }
  if (current_group_ == groups_.end()) {
    return RC::RECORD_EOF;
  }
  // 如果 is_null 为 true，说明表为空，应该返回 EOF，而不是 NULL_RECORD
  // 因为空表的 GROUP BY 应该返回空结果（0行），而不是返回一行 NULL 值
  if(is_null) {
    return RC::RECORD_EOF;
  }
  return RC::SUCCESS;
}