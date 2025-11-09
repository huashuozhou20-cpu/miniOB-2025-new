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
// Created by Wangyunlai on 2024/12/07
//

#pragma once

#include "sql/operator/logical_operator.h"

/**
 * @brief UNION 逻辑算子
 * @ingroup LogicalOperator
 * @details UNION 算子用于合并两个 SELECT 查询的结果
 */
class UnionLogicalOperator : public LogicalOperator
{
public:
  UnionLogicalOperator(bool union_all) : union_all_(union_all) {}
  virtual ~UnionLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UNION; }
  
  bool union_all() const { return union_all_; }

private:
  bool union_all_;  ///< 是否是 UNION ALL（不去重）
};

