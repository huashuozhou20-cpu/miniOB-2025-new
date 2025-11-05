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

#pragma once

#include <unordered_set>
#include <vector>
#include "sql/operator/physical_operator.h"
#include "sql/operator/value_tuple.h"
#include "sql/expr/tuple.h"

/**
 * @brief UNION 物理算子
 * @details 合并两个 SELECT 查询的结果，支持 UNION（去重）和 UNION ALL（不去重）
 * @ingroup PhysicalOperator
 */
class UnionPhysicalOperator : public PhysicalOperator
{
public:
  UnionPhysicalOperator(bool union_all);
  virtual ~UnionPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UNION; }

  RC     open(Trx *trx) override;
  RC     next() override;
  RC     close() override;
  Tuple *current_tuple() override;
  RC     tuple_schema(TupleSchema &schema) const override;

private:
  RC fetch_all_tuples();  ///< 从两个子算子获取所有结果
  bool tuple_equal(const Tuple &t1, const Tuple &t2) const;  ///< 比较两个 tuple 是否相等
  std::string tuple_to_string(const Tuple &tuple) const;  ///< 将 tuple 转换为字符串用于去重

private:
  Trx *trx_ = nullptr;
  bool union_all_;  ///< 是否是 UNION ALL（不去重）
  
  PhysicalOperator *left_  = nullptr;   ///< 左子算子
  PhysicalOperator *right_ = nullptr;   ///< 右子算子
  
  std::vector<std::vector<Value>> all_values_;  ///< 存储所有结果的值
  std::unordered_set<std::string> seen_tuples_;  ///< 用于 UNION 去重
  size_t current_index_ = 0;            ///< 当前输出的索引
  bool initialized_ = false;             ///< 是否已经初始化
  mutable ValueTuple current_tuple_;       ///< 当前输出的 tuple
  TupleSchema schema_;                  ///< Tuple 的 schema
};

