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

#include <vector>
#include "sql/expr/tuple.h"
#include "common/value.h"

/**
 * @brief 简单的值 Tuple，用于存储值列表
 * @ingroup Tuple
 */
class ValueTuple : public Tuple
{
public:
  ValueTuple() = default;
  virtual ~ValueTuple() = default;

  void set_values(const std::vector<Value> &values) { values_ = values; }

  int cell_num() const override { return static_cast<int>(values_.size()); }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::INVALID_ARGUMENT;
    }
    cell = values_[index];
    return RC::SUCCESS;
  }

  RC spec_at(int index, TupleCellSpec &spec) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::INVALID_ARGUMENT;
    }
    spec = TupleCellSpec("");
    return RC::SUCCESS;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    return RC::NOTFOUND;
  }

  RC get_tuple_rid(int id, const BaseTable *&table, RID &rid) const override
  {
    return RC::INVALID_ARGUMENT;
  }

  int get_tuple_size() const override
  {
    return 0;
  }

private:
  std::vector<Value> values_;
};

