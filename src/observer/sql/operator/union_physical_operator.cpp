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
// Created by WangYunlai on 2024/12/07.
//

#include "sql/operator/union_physical_operator.h"
#include "common/log/log.h"
#include "common/sys/rc.h"
#include <sstream>

UnionPhysicalOperator::UnionPhysicalOperator(bool union_all) : union_all_(union_all) {}

RC UnionPhysicalOperator::open(Trx *trx)
{
  LOG_INFO("open union operator, union_all=%d", union_all_);
  
  if (children_.size() != 2) {
    LOG_WARN("union operator should have 2 children");
    return RC::INTERNAL;
  }

  trx_ = trx;
  left_ = children_[0].get();
  right_ = children_[1].get();

  RC rc = left_->open(trx);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to open left child. rc=%s", strrc(rc));
    return rc;
  }

  rc = right_->open(trx);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to open right child. rc=%s", strrc(rc));
    left_->close();
    return rc;
  }

  // 获取所有结果
  rc = fetch_all_tuples();
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to fetch all tuples. rc=%s", strrc(rc));
    return rc;
  }

  initialized_ = true;
  current_index_ = 0;

  return RC::SUCCESS;
}

RC UnionPhysicalOperator::next()
{
  if (!initialized_) {
    return RC::INTERNAL;
  }

  if (current_index_ >= all_values_.size()) {
    return RC::RECORD_EOF;
  }

  current_index_++;
  return RC::SUCCESS;
}

RC UnionPhysicalOperator::close()
{
  LOG_INFO("close union operator");
  
  RC rc = RC::SUCCESS;
  if (left_ != nullptr) {
    RC rc2 = left_->close();
    if (OB_FAIL(rc2)) {
      LOG_WARN("failed to close left child. rc=%s", strrc(rc2));
      rc = rc2;
    }
  }

  if (right_ != nullptr) {
    RC rc2 = right_->close();
    if (OB_FAIL(rc2)) {
      LOG_WARN("failed to close right child. rc=%s", strrc(rc2));
      rc = rc2;
    }
  }

  // 清理所有数据
  all_values_.clear();
  current_index_ = 0;
  initialized_ = false;

  return rc;
}

Tuple *UnionPhysicalOperator::current_tuple()
{
  if (!initialized_ || current_index_ == 0 || current_index_ > all_values_.size()) {
    return nullptr;
  }

  // 创建 ValueTuple 用于当前输出
  current_tuple_.set_values(all_values_[current_index_ - 1]);
  return &current_tuple_;
}

RC UnionPhysicalOperator::tuple_schema(TupleSchema &schema) const
{
  // UNION 的 schema 应该与左子算子的 schema 相同
  if (children_.empty()) {
    return RC::INTERNAL;
  }

  RC rc = children_[0]->tuple_schema(schema);
  return rc;
}

RC UnionPhysicalOperator::fetch_all_tuples()
{
  all_values_.clear();

  // 获取 schema（从左子算子）
  RC rc = left_->tuple_schema(schema_);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to get tuple schema from left child. rc=%s", strrc(rc));
    return rc;
  }

  // 从左子算子获取所有结果
  while ((rc = left_->next()) == RC::SUCCESS) {
    Tuple *tuple = left_->current_tuple();
    if (tuple == nullptr) {
      continue;
    }

    // 提取所有值
    std::vector<Value> values;
    int cell_num = tuple->cell_num();
    for (int i = 0; i < cell_num; i++) {
      Value value;
      RC rc2 = tuple->cell_at(i, value);
      if (OB_FAIL(rc2)) {
        LOG_WARN("failed to get cell value. rc=%s", strrc(rc2));
        return rc2;
      }
      values.push_back(value);
    }

    if (union_all_) {
      // UNION ALL：直接添加，不去重
      all_values_.push_back(std::move(values));
    } else {
      // UNION：需要去重，使用 tuple_equal 进行比较
      bool is_duplicate = false;
      for (const auto &existing_values : all_values_) {
        // 创建临时的 ValueTuple 进行比较
        ValueTuple temp_tuple;
        temp_tuple.set_values(existing_values);
        if (tuple_equal(*tuple, temp_tuple)) {
          is_duplicate = true;
          break;
        }
      }
      if (!is_duplicate) {
        all_values_.push_back(std::move(values));
      }
    }
  }

  if (rc != RC::RECORD_EOF) {
    LOG_WARN("failed to read from left child. rc=%s", strrc(rc));
    return rc;
  }

  // 从右子算子获取所有结果
  while ((rc = right_->next()) == RC::SUCCESS) {
    Tuple *tuple = right_->current_tuple();
    if (tuple == nullptr) {
      continue;
    }

    // 提取所有值
    std::vector<Value> values;
    int cell_num = tuple->cell_num();
    for (int i = 0; i < cell_num; i++) {
      Value value;
      RC rc2 = tuple->cell_at(i, value);
      if (OB_FAIL(rc2)) {
        LOG_WARN("failed to get cell value. rc=%s", strrc(rc2));
        return rc2;
      }
      values.push_back(value);
    }

    if (union_all_) {
      // UNION ALL：直接添加，不去重
      all_values_.push_back(std::move(values));
    } else {
      // UNION：需要去重，使用 tuple_equal 进行比较
      bool is_duplicate = false;
      for (const auto &existing_values : all_values_) {
        // 创建临时的 ValueTuple 进行比较
        ValueTuple temp_tuple;
        temp_tuple.set_values(existing_values);
        if (tuple_equal(*tuple, temp_tuple)) {
          is_duplicate = true;
          break;
        }
      }
      if (!is_duplicate) {
        all_values_.push_back(std::move(values));
      }
    }
  }

  if (rc != RC::RECORD_EOF) {
    LOG_WARN("failed to read from right child. rc=%s", strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

bool UnionPhysicalOperator::tuple_equal(const Tuple &t1, const Tuple &t2) const
{
  int cell_num1 = t1.cell_num();
  int cell_num2 = t2.cell_num();

  if (cell_num1 != cell_num2) {
    return false;
  }

  for (int i = 0; i < cell_num1; i++) {
    Value value1, value2;
    RC rc1 = t1.cell_at(i, value1);
    RC rc2 = t2.cell_at(i, value2);

    if (rc1 != RC::SUCCESS || rc2 != RC::SUCCESS) {
      return false;
    }

    // Handle NULL values explicitly
    if (value1.attr_type() == AttrType::NULLS || value2.attr_type() == AttrType::NULLS) {
      if (value1.attr_type() != value2.attr_type()) {
        return false;
      }
      // Both are NULL, consider them equal
      continue;
    }

    if (value1.compare(value2) != 0) {
      return false;
    }
  }

  return true;
}

std::string UnionPhysicalOperator::tuple_to_string(const Tuple &tuple) const
{
  std::ostringstream oss;
  int cell_num = tuple.cell_num();

  for (int i = 0; i < cell_num; i++) {
    if (i > 0) {
      oss << "|";
    }

    Value value;
    RC rc = tuple.cell_at(i, value);
    if (rc != RC::SUCCESS) {
      oss << "NULL";
      continue;
    }

    if (value.attr_type() == AttrType::NULLS) {
      oss << "NULL";
    } else {
      switch (value.attr_type()) {
        case AttrType::INTS:
          oss << value.get_int();
          break;
        case AttrType::FLOATS:
          oss << value.get_float();
          break;
        case AttrType::CHARS:
          oss << value.get_string();
          break;
        case AttrType::DATES:
          oss << value.get_string();
          break;
        default:
          oss << "UNKNOWN";
          break;
      }
    }
  }

  return oss.str();
}

