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
// Created by Wangyunlai on 2024/5/31.
//

#pragma once

#include "common/lang/vector.h"
#include "sql/expr/tuple.h"
#include "sql/expr/expression.h"
#include <type_traits>
#include "common/value.h"
#include "common/sys/rc.h"

template <typename ExprPointerType>
class ExpressionTuple : public Tuple
{
public:
  ExpressionTuple(const vector<ExprPointerType> &expressions) : expressions_(expressions) {}
  virtual ~ExpressionTuple() = default;

  void set_tuple(const Tuple *tuple) { child_tuple_ = tuple; }

  int cell_num() const override { return static_cast<int>(expressions_.size()); }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::INVALID_ARGUMENT;
    }

    const ExprPointerType &expression = expressions_[index];
    return get_value(expression, cell);
  }

  RC spec_at(int index, TupleCellSpec &spec) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::INVALID_ARGUMENT;
    }

    const ExprPointerType &expression = expressions_[index];
    const Expression      *expr_ptr   = nullptr;
    if constexpr (std::is_pointer_v<ExprPointerType>) {
      expr_ptr = expression;
    } else {
      expr_ptr = expression.get();
    }

    if (expr_ptr == nullptr) {
      return RC::INVALID_ARGUMENT;
    }

    const string &alias = expr_ptr->alias();

    if (expr_ptr->type() == ExprType::FIELD) {
      const FieldExpr *field_expr = static_cast<const FieldExpr *>(expr_ptr);
      const char      *table_name = nullptr;
      string           table_holder;
      if (!field_expr->table_alias().empty()) {
        table_holder = field_expr->table_alias();
        table_name   = table_holder.c_str();
      } else {
        table_name = field_expr->table_name();
      }
      const char *field_name = field_expr->field_name();
      if (alias.empty()) {
        spec = TupleCellSpec(table_name, field_name, expr_ptr->name());
      } else {
        spec = TupleCellSpec(table_name, field_name, alias.c_str());
      }
    } else {
      if (alias.empty()) {
        spec = TupleCellSpec(expr_ptr->name());
      } else {
        spec = TupleCellSpec(alias.c_str());
      }
    }
    return RC::SUCCESS;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    RC rc = RC::SUCCESS;
    if (child_tuple_ != nullptr) {
      rc = child_tuple_->find_cell(spec, cell);
      if (OB_SUCC(rc)) {
        return rc;
      }
    }

    rc = RC::NOTFOUND;
    for (const ExprPointerType &expression : expressions_) {
      if (0 == strcmp(spec.alias(), expression->name())) {
        rc = get_value(expression, cell);
        break;
      }
    }

    return rc;
  }

  int get_tuple_size() const override
  {
    if(child_tuple_ == nullptr)return 0;
    return child_tuple_->get_tuple_size();
  }

  RC get_tuple_rid(int id, const BaseTable *&table, RID &rid) const override
  {
    if(child_tuple_ == nullptr)return RC::INVALID_ARGUMENT;
    return child_tuple_->get_tuple_rid(id, table, rid);
  }

private:
  RC get_value(const ExprPointerType &expression, Value &value) const
  {
    RC rc = RC::SUCCESS;
    if (child_tuple_ != nullptr) {
      rc = expression->get_value(*child_tuple_, value);
      // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
      if (rc == RC::NULL_TUPLE) {
        value.set_null();
        rc = RC::SUCCESS;
      }
    } else {
      rc = expression->try_get_value(value);
      // Handle RC::NULL_TUPLE: if try_get_value returns NULL_TUPLE, set value to NULL
      if (rc == RC::NULL_TUPLE) {
        value.set_null();
        rc = RC::SUCCESS;
      }
    }
    return rc;
  }

private:
  const vector<ExprPointerType> &expressions_;
  const Tuple                   *child_tuple_ = nullptr;
};
