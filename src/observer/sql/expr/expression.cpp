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
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/arithmetic_operator.hpp"
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/optimizer/logical_plan_generator.h"
#include "sql/executor/sql_result.h"
#include "sql/stmt/select_stmt.h"
#include "storage/index/fulltext_index.h"
#include "storage/index/index.h"
#include "storage/table/table.h"
#include <cmath>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

RC Expression::recursion(std::unique_ptr<Expression>& expr, 
    const std::function<RC(std::unique_ptr<Expression>&)>& func)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  switch (expr->type())
  {
    case ExprType::ARITHMETIC:{
      auto arithmetic_expr = static_cast<ArithmeticExpr*>(expr.get());

       unique_ptr<Expression>        &left_expr  = arithmetic_expr->left();
       unique_ptr<Expression>        &right_expr = arithmetic_expr->right();

      RC rc = recursion(left_expr, func);
      if (OB_FAIL(rc)) {
        return rc;
      }

      rc = recursion(right_expr, func);
      if (OB_FAIL(rc)) {
        return rc;
      }
      return RC::SUCCESS;
    }break;
    case ExprType::VECTOROPERATION:{
      auto operation_expr = static_cast<VectorOperationExpr*>(expr.get());

       unique_ptr<Expression>        &left_expr  = operation_expr->left();
       unique_ptr<Expression>        &right_expr = operation_expr->right();

      RC rc = recursion(left_expr, func);
      if (OB_FAIL(rc)) {
        return rc;
      }

      rc = recursion(right_expr, func);
      if (OB_FAIL(rc)) {
        return rc;
      }
      return RC::SUCCESS;
    }break;
    case ExprType::COMPARISON:{
      auto comparison_expr = static_cast<ComparisonExpr*>(expr.get());
      unique_ptr<Expression> child_bound_expression;
      unique_ptr<Expression>        &left_expr  = comparison_expr->left();
      unique_ptr<Expression>        &right_expr = comparison_expr->right();
  
      RC rc = recursion(left_expr, func);
      if (rc != RC::SUCCESS) {
        return rc;
      }

      rc = recursion(right_expr, func);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      return RC::SUCCESS;
    }break;
    case ExprType::SYSFUNC:{
      auto sysfunc_expr = static_cast<SysFuncExpr*>(expr.get());
       unique_ptr<Expression>        &child_expr  = sysfunc_expr->child();
       unique_ptr<Expression>        &second_child_expr = sysfunc_expr->second_child();

      RC rc = recursion(child_expr, func);
      if (OB_FAIL(rc)) {
        return rc;
      }

      if (second_child_expr) {
        rc = recursion(second_child_expr, func);
        if (OB_FAIL(rc)) {
          return rc;
        }
      }
      return RC::SUCCESS;
    }break;
    default:return func(expr);
  }
  return RC::SUCCESS;
}

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  if(table_alias_.empty())
    return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
  return tuple.find_cell(TupleCellSpec(table_alias_.c_str(), field_name()), value);
}

bool FieldExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::FIELD) {
    return false;
  }
  const auto &other_field_expr = static_cast<const FieldExpr &>(other);
  return table_name() == other_field_expr.table_name() && field_name() == other_field_expr.field_name();
}

// TODO: 在进行表达式计算时，`chunk` 包含了所有列，因此可以通过 `field_id` 获取到对应列。
// 后续可以优化成在 `FieldExpr` 中存储 `chunk` 中某列的位置信息。
RC FieldExpr::get_column(Chunk &chunk, Column &column)
{
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    column.reference(chunk.column(field().meta()->field_id()));
  }
  return RC::SUCCESS;
}

bool ValueExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::VALUE) {
    return false;
  }
  const auto &other_value_expr = static_cast<const ValueExpr &>(other);
  return value_.compare(other_value_expr.get_value()) == 0;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueExpr::get_column(Chunk &chunk, Column &column)
{
  column.init(value_, chunk.rows());
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type) : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }
  rc = Value::cast_to(value, cast_type_, cast_value);
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &result) const
{
  Value value;
  RC rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

RC CastExpr::get_column(Chunk &chunk, Column &column)
{
  Column child_column;
  RC rc = child_->get_column(chunk, child_column);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  column.init(cast_type_, child_column.attr_len());
  for (int i = 0; i < child_column.count(); ++i) {
    Value value = child_column.get_value(i);
    Value cast_value;
    rc = cast(value, cast_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    column.append_value(cast_value);
  }
  return rc;
}

RC CastExpr::get_value_set(const Tuple &tuple, vector<Value> &value_list)const 
{
  RC rc = child_->get_value_set(tuple, value_list);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  Value temp;
  for(auto& value : value_list){
    rc = cast(value, temp);
    if(rc != RC::SUCCESS)return rc;
    value = move(temp);
  }
  return rc;
}

RC CastExpr::try_get_value(Value &result) const
{
  Value value;
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right)), pattern("")
{
  if((comp_ == CompOp::LIKE_OP || comp_ == CompOp::NOT_LIKE)
     && right_->type() == ExprType::VALUE){
    Value pattern_val;
    static_cast<ValueExpr*>(right_.get())->get_value(pattern_val);
    string right_string = pattern_val.get_string();
    string pattern_string;
    for(auto& c : right_string){
      switch (c)
      {
      case '_':pattern_string.append(1, '.');break;
      case '%':pattern_string.append(".*");break;
      case '*':case '.':pattern_string.append(1, '\\');
      default:pattern_string.append(1, c);
        break;
      }
    }
    pattern.assign(std::move(pattern_string));
  }
}

ComparisonExpr::~ComparisonExpr() {}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  if(left.attr_type() == AttrType::NULLS || right.attr_type() == AttrType::NULLS){
    result = false;
    return RC::SUCCESS;
  }
  RC  rc         = RC::SUCCESS;
  int cmp_result = left.compare(right);
  result         = false;
  switch (comp_) {
    case EQUAL_TO: {
      result = (0 == cmp_result);
    } break;
    case LESS_EQUAL: {
      result = (cmp_result <= 0);
    } break;
    case NOT_EQUAL: {
      result = (cmp_result != 0);
    } break;
    case LESS_THAN: {
      result = (cmp_result < 0);
    } break;
    case GREAT_EQUAL: {
      result = (cmp_result >= 0);
    } break;
    case GREAT_THAN: {
      result = (cmp_result > 0);
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::like_value(const Tuple &tuple, bool &result) const
{
  if(right_->type() != ExprType::VALUE)return RC::INVALID_ARGUMENT;

  Value left_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if(left_value.attr_type() == AttrType::NULLS)
  {
    result = false;
    return RC::SUCCESS;
  }

  result = regex_match(left_value.get_string(), pattern);
  return RC::SUCCESS;
}

RC ComparisonExpr::value_exists(const Tuple &tuple, bool &result) const
{
  if(right_->type() != ExprType::SELECT)return RC::INVALID_ARGUMENT;

  Value right_value;

  RC rc = right_->get_value(tuple, right_value);
  
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE && rc != RC::MUTI_TUPLE) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  result = !(rc == RC::NULL_TUPLE); 
  return RC::SUCCESS;
}

RC ComparisonExpr::value_in(const Tuple &tuple, bool &result) const
{
  if(right_->type() != ExprType::SELECT && right_->type() != ExprType::VALUE_LIST)
    return RC::INVALID_ARGUMENT;

  result = false;
  Value left_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  if(left_value.attr_type() == AttrType::NULLS)return RC::SUCCESS;

  vector<Value> value_list;
  rc = right_->get_value_set(tuple, value_list);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  for(auto& value : value_list){
    if(value.attr_type() != AttrType::NULLS && left_value.compare(value) == 0){
      result = true;
      break;
    }
  }

  return RC::SUCCESS;
}

RC ComparisonExpr::value_not_in(const Tuple &tuple, bool &result) const
{
  if(right_->type() != ExprType::SELECT && right_->type() != ExprType::VALUE_LIST)
    return RC::INVALID_ARGUMENT;

  result = false;
  Value left_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  if(left_value.attr_type() == AttrType::NULLS)return RC::SUCCESS;

  vector<Value> value_list;
  rc = right_->get_value_set(tuple, value_list);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  for(auto& value : value_list){
    if(value.attr_type() == AttrType::NULLS)
    {
      result = false;
      return RC::SUCCESS;
    }
    else if(left_value.compare(value) == 0){
      result = true;
    }
  }
  result = !result;

  return RC::SUCCESS;
}

RC ComparisonExpr::value_is_null(const Tuple &tuple, bool &result) const
{
  if(right_->type() != ExprType::VALUE || right_->value_type() != AttrType::NULLS)
    return RC::INVALID_ARGUMENT;

  Value left_value;
  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 如果返回 RC::NULL_TUPLE，说明左值是 NULL
  if (rc == RC::NULL_TUPLE) {
    left_value.set_null();
  }
  result = (left_value.attr_type() == AttrType::NULLS);
  
  return RC::SUCCESS;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *  left_value_expr  = static_cast<ValueExpr *>(left_.get());
    ValueExpr *  right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell        = left_value_expr->get_value();
    const Value &right_cell       = right_value_expr->get_value();

    bool value = false;
    if(comp_ == CompOp::IS_NULL || comp_ == CompOp::IS_NOT_NULL){
      if(right_cell.attr_type() != AttrType::NULLS)return RC::INVALID_ARGUMENT;
      if((comp_ == CompOp::IS_NULL && left_cell.attr_type() == AttrType::NULLS) || 
        (comp_ == CompOp::IS_NOT_NULL && left_cell.attr_type() != AttrType::NULLS))
          cell.set_boolean(true);
      else cell.set_boolean(false);
      return RC::SUCCESS;
    }
    RC   rc    = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  bool bool_value = false;
  
  switch (comp_)
  {
    case CompOp::LIKE_OP:{
      rc = like_value(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
    case CompOp::NOT_LIKE:{
      rc = like_value(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(!bool_value);
      }
    }break;
    case CompOp::EXISTS_OP:{
      rc = value_exists(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
    case CompOp::NOT_EXISTS:{
      rc = value_exists(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(!bool_value);
      }
    }break;
    case CompOp::IN_OP:{
      rc = value_in(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
    case CompOp::NOT_IN:{
      rc = value_not_in(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
    case CompOp::IS_NULL:{
      rc = value_is_null(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
    case CompOp::IS_NOT_NULL:{
      rc = value_is_null(tuple, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(!bool_value);
      }
    }break;
    default:{
      Value left_value;
      Value right_value;

      rc = left_->get_value(tuple, left_value);
      if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
        LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
        return rc;
      }
      // 如果返回 RC::NULL_TUPLE，说明左值是 NULL
      if (rc == RC::NULL_TUPLE) {
        left_value.set_null();
      }

      rc = right_->get_value(tuple, right_value);
      if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
        LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
        return rc;
      }
      // 如果返回 RC::NULL_TUPLE，说明右值是 NULL
      if (rc == RC::NULL_TUPLE) {
        right_value.set_null();
      }

      rc = compare_value(left_value, right_value, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
    }break;
  }
  return rc;
}

RC ComparisonExpr::eval(Chunk &chunk, vector<uint8_t> &select)
{
  RC     rc = RC::SUCCESS;
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  if (left_column.attr_type() != right_column.attr_type()) {
    LOG_WARN("cannot compare columns with different types");
    return RC::INTERNAL;
  }
  if (left_column.attr_type() == AttrType::INTS) {
    rc = compare_column<int>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::FLOATS) {
    rc = compare_column<float>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::CHARS) {
    int rows = 0;
    if (left_column.column_type() == Column::Type::CONSTANT_COLUMN) {
      rows = right_column.count();
    } else {
      rows = left_column.count();
    }
    for (int i = 0; i < rows; ++i) {
      Value left_val = left_column.get_value(i);
      Value right_val = right_column.get_value(i);
      bool        result   = false;
      rc                   = compare_value(left_val, right_val, result);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
        return rc;
      }
      select[i] &= result ? 1 : 0;
    }

  } else {
    LOG_WARN("unsupported data type %d", left_column.attr_type());
    return RC::INTERNAL;
  }
  return rc;
}

template <typename T>
RC ComparisonExpr::compare_column(const Column &left, const Column &right, vector<uint8_t> &result) const
{
  RC rc = RC::SUCCESS;

  bool left_const  = left.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    compare_result<T, true, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else if (left_const && !right_const) {
    compare_result<T, true, false>((T *)left.data(), (T *)right.data(), right.count(), result, comp_);
  } else if (!left_const && right_const) {
    compare_result<T, false, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else {
    compare_result<T, false, false>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
    if (rc == RC::NULL_TUPLE) {
      tmp_value.set_null();
    }
    // Handle NULL values: if value is NULL, AND returns false, OR continues
    if (tmp_value.attr_type() == AttrType::NULLS) {
      if (conjunction_type_ == Type::AND) {
        value.set_boolean(false);
        return rc;
      } else {
        // For OR, continue to next expression
        continue;
      }
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

bool ArithmeticExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_arith_expr = static_cast<const ArithmeticExpr &>(other);
  return arithmetic_type_ == other_arith_expr.arithmetic_type() && left_->equal(*other_arith_expr.left_) &&
         right_->equal(*other_arith_expr.right_);
}

AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if(left_->value_type() == AttrType::VECTORS || right_->value_type() == AttrType::VECTORS){
    return AttrType::VECTORS;
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }

  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;
  
  const AttrType target_type = value_type();
  value.set_type(target_type);

  switch (arithmetic_type_) {
    case Type::ADD: {
      Value::add(left_value, right_value, value);
    } break;

    case Type::SUB: {
      Value::subtract(left_value, right_value, value);
    } break;

    case Type::MUL: {
      Value::multiply(left_value, right_value, value);
    } break;

    case Type::DIV: {
      Value::divide(left_value, right_value, value);
    } break;

    case Type::NEGATIVE: {
      Value::negative(left_value, value);
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
RC ArithmeticExpr::execute_calc(
    const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const
{
  RC rc = RC::SUCCESS;
  switch (type) {
    case Type::ADD: {
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, AddOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, AddOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
    } break;
    case Type::SUB:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, SubtractOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, SubtractOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::MUL:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, MultiplyOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, MultiplyOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::DIV:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, DivideOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, DivideOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::NEGATIVE:
      if (attr_type == AttrType::INTS) {
        unary_operator<LEFT_CONSTANT, int, NegateOperator>((int *)left.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        unary_operator<LEFT_CONSTANT, float, NegateOperator>(
            (float *)left.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    default: rc = RC::UNIMPLEMENTED; break;
  }
  if (rc == RC::SUCCESS) {
    result.set_count(result.capacity());
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 如果返回 RC::NULL_TUPLE，说明左值是 NULL
  if (rc == RC::NULL_TUPLE) {
    left_value.set_null();
  }

  if(right_){
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
    // 如果返回 RC::NULL_TUPLE，说明右值是 NULL
    if (rc == RC::NULL_TUPLE) {
      right_value.set_null();
    }
  }
  
  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
    return rc;
  }
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_column(left_column, right_column, column);
}

RC ArithmeticExpr::calc_column(const Column &left_column, const Column &right_column, Column &column) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  column.init(target_type, left_column.attr_len(), max(left_column.count(), right_column.count()));
  bool left_const  = left_column.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right_column.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    column.set_column_type(Column::Type::CONSTANT_COLUMN);
    rc = execute_calc<true, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (left_const && !right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<true, false>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (!left_const && right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, false>(left_column, right_column, column, arithmetic_type_, target_type);
  }
  return rc;
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

VectorOperationExpr::VectorOperationExpr(VectorOperationExpr::Type type, Expression *left, Expression *right)
    : operation_type_(type), left_(left), right_(right)
{}
VectorOperationExpr::VectorOperationExpr(VectorOperationExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : operation_type_(type), left_(std::move(left)), right_(std::move(right))
{}

bool VectorOperationExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_operation_expr = static_cast<const VectorOperationExpr &>(other);
  return operation_type_ == other_operation_expr.operation_type() && left_->equal(*other_operation_expr.left_) &&
         right_->equal(*other_operation_expr.right_);
}

AttrType VectorOperationExpr::value_type() const
{
  //是向量特有操作，向量直接储存在数组里，直接返回数组类型
  return AttrType::VECTORS;
}

unique_ptr<Expression> VectorOperationExpr::copy() const
{
  return make_unique<VectorOperationExpr>(operation_type_, left_->copy(), right_->copy());
}

RC VectorOperationExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if(right_){
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }
  
  return calc_value(left_value, right_value, value);
}

RC VectorOperationExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}
  
RC VectorOperationExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;
  
  const AttrType target_type = value_type();
  value.set_type(target_type);

  switch (operation_type_) {
    case Type::L2_DISTANCE: {
      Value::l2_distance(left_value, right_value, value);
    } break;

    case Type::COSINE_DISTANCE: {
      Value::cosine_distance(left_value, right_value, value);
    } break;

    case Type::INNER_PRODUCT: {
      Value::inner_product(left_value, right_value, value);
    } break;
    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", operation_type_);
    } break;
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, Expression *child)
    : aggregate_name_(aggregate_name), child_(child)
{}

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, unique_ptr<Expression> child)
    : aggregate_name_(aggregate_name), child_(std::move(child))
{}

////////////////////////////////////////////////////////////////////////////////
AggregateExpr::AggregateExpr(Type type, Expression *child) : aggregate_type_(type), child_(child) {}

AggregateExpr::AggregateExpr(Type type, unique_ptr<Expression> child) : aggregate_type_(type), child_(std::move(child))
{}

RC AggregateExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    rc = RC::INTERNAL;
  }
  return rc;
}

bool AggregateExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const AggregateExpr &other_aggr_expr = static_cast<const AggregateExpr &>(other);
  return aggregate_type_ == other_aggr_expr.aggregate_type() && child_->equal(*other_aggr_expr.child());
}

unique_ptr<Aggregator> AggregateExpr::create_aggregator() const
{
  unique_ptr<Aggregator> aggregator;
  switch (aggregate_type_) {
    case Type::SUM: {
      aggregator = make_unique<SumAggregator>();
      break;
    }
    case Type::COUNT: {
      aggregator = make_unique<CountAggregator>();
      break;
    }
    case Type::MAX: {
      aggregator = make_unique<MaxAggregator>();
      break;
    }
    case Type::MIN: {
      aggregator = make_unique<MinAggregator>();
      break;
    }
    case Type::AVG: {
      aggregator = make_unique<AvgAggregator>();
      break;
    }
    default: {
      ASSERT(false, "unsupported aggregate type");
      break;
    }
  }
  return aggregator;
}

RC AggregateExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(name()), value);
}

RC AggregateExpr::type_from_string(const char *type_str, AggregateExpr::Type &type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "count")) {
    type = Type::COUNT;
  } else if (0 == strcasecmp(type_str, "sum")) {
    type = Type::SUM;
  } else if (0 == strcasecmp(type_str, "avg")) {
    type = Type::AVG;
  } else if (0 == strcasecmp(type_str, "max")) {
    type = Type::MAX;
  } else if (0 == strcasecmp(type_str, "min")) {
    type = Type::MIN;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

RC SelectExpr::logical_generate()
{
  if(logical_operator_ != nullptr)return RC::SUCCESS;
  RC rc = RC::SUCCESS;
  rc = LogicalPlanGenerator().create(stmt_, logical_operator_);
  value_type_ = logical_operator_->expressions().at(0)->value_type();
  return rc;
}

SelectExpr::SelectExpr(ParsedSqlNode* sql_node) : sql_node_(sql_node){}

RC SelectExpr::physical_generate()
{
  if(physical_operator_ != nullptr)return RC::SUCCESS;
  return PhysicalPlanGenerator().create(*logical_operator_, physical_operator_);
}

SelectExpr::~SelectExpr(){
  if(stmt_ != nullptr)delete stmt_;
  if(sql_node_ != nullptr)delete sql_node_;
}

RC SelectExpr::create_stmt(Db *db, vector<vector<uint32_t>>& depends, vector<SelectExpr*>& select_exprs,
  tables_t& table_map, int fa)
{
  return Stmt::create_stmt(db, *sql_node_, stmt_, depends, select_exprs, table_map, fa);
}

unique_ptr<Expression> SelectExpr::copy() const
{
  // SelectExpr cannot be copied easily as it contains complex state
  // Return a new SelectExpr with the same sql_node if available
  if (sql_node_ != nullptr) {
    return make_unique<SelectExpr>(sql_node_);
  }
  return make_unique<SelectExpr>();
}

RC SelectExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if(values_ != nullptr){
    if(values_->size() == 0 || values_->at(0).size() == 0){
      value.set_null();
      return RC::NULL_TUPLE;
    }
    if(values_->size() > 1 || values_->at(0).size() > 1)
      return RC::MUTI_TUPLE;
    value = values_->at(0)[0];
    return rc;
  }

  physical_operator_->open(trx_);
  Tuple *ret_tuple = nullptr;
  auto temp = const_cast<Tuple*>(&tuple);
  int num = 0;
  while (RC::SUCCESS == (rc = next_tuple(ret_tuple, temp))) {   
    assert(ret_tuple != nullptr);

    num++;
    if(num > 1){
      physical_operator_->close();
      return RC::MUTI_TUPLE;
    }
    
    ret_tuple->cell_at(0, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get tuple cell value. rc=%s", strrc(rc));
      return rc;
    }
  }

  physical_operator_->close();

  if (rc == RC::RECORD_EOF) {
    if(num == 0){
      value.set_null();
      return RC::NULL_TUPLE;
    }
    rc = RC::SUCCESS;
  }
  return rc;
}

RC SelectExpr::get_value_set(const Tuple &tuple, vector<Value> &value_list) const
{
  RC rc = RC::SUCCESS;
  
  if(values_ != nullptr){
    for(auto& values : *values_){
      if(values.size() == 0)return RC::INVALID_ARGUMENT;
      if(values.size() > 1)return RC::INVALID_ARGUMENT;
      value_list.emplace_back(values[0]);
    }
    return rc;
  }

  physical_operator_->open(trx_);
  Tuple *ret_tuple = nullptr;
  auto temp = const_cast<Tuple*>(&tuple);

  Value value_get;
  while (RC::SUCCESS == (rc = next_tuple(ret_tuple, temp))) {   
    assert(ret_tuple != nullptr);
    
    rc = ret_tuple->cell_at(0, value_get);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get tuple cell value. rc=%s", strrc(rc));
      return rc;
    }

    value_list.emplace_back(value_get);
  }

  physical_operator_->close();

  if (rc == RC::RECORD_EOF) {
    rc = RC::SUCCESS;
  }
  return rc;
}

RC SelectExpr::next_tuple(Tuple *&tuple, Tuple *upper_tuple) const
{
  RC rc = RC::SUCCESS;
  if(upper_tuple == nullptr)
    rc = physical_operator_->next();
  else rc = physical_operator_->next(upper_tuple); 
  if (rc != RC::SUCCESS) {
    return rc;
  }

  tuple = physical_operator_->current_tuple();
  return rc;
}

RC SelectExpr::pretreatment()
{
  RC rc = RC::SUCCESS;
  Tuple *tuple = nullptr;
  physical_operator_->open(trx_);
  
  values_.reset();
  values_ = make_unique<vector<vector<Value>>>();

  Value value;
  while (RC::SUCCESS == (rc = next_tuple(tuple))) {
    assert(tuple != nullptr);

    int cell_num = tuple->cell_num();
    auto size = values_->size();
    values_->emplace_back(vector<Value>());

    for (int i = 0; i < cell_num; i++) {
      rc = tuple->cell_at(i, value);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get tuple cell value. rc=%s", strrc(rc));
        return rc;
      }
      values_->at(size).emplace_back(move(value));
    }
  }

  physical_operator_->close();

  if (rc == RC::RECORD_EOF) {
    rc = RC::SUCCESS;
  }

  return rc;
}

unique_ptr<Expression> ValueListExpr::copy() const
{
  vector<unique_ptr<Expression>> copied_exprs;
  for (const auto &expr : exprs_) {
    copied_exprs.push_back(expr->copy());
  }
  return make_unique<ValueListExpr>(copied_exprs);
}

RC ValueListExpr::get_value(const Tuple &tuple, Value &value) const 
{
  return exprs_[0]->get_value(tuple, value);
}

RC ValueListExpr::get_value_set(const Tuple &tuple, vector<Value> &value_list) const
{
  RC rc = RC::SUCCESS;
  Value value;
  for(auto& expr : exprs_){
    rc = expr->get_value(tuple, value);
    if(rc != RC::SUCCESS)return rc;
    value_list.emplace_back(move(value));
  }
  return RC::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

UnboundSysFuncExpr::UnboundSysFuncExpr(const char *func_name, Expression *child)
    : func_name_(func_name), child_(child), second_child_(nullptr)
{}

UnboundSysFuncExpr::UnboundSysFuncExpr(const char *func_name, Expression *child, Expression *second_child)
    : func_name_(func_name), child_(child), second_child_(second_child), third_child_(nullptr)
{}

UnboundSysFuncExpr::UnboundSysFuncExpr(const char *func_name, Expression *child, Expression *second_child, Expression *third_child)
    : func_name_(func_name), child_(child), second_child_(second_child), third_child_(third_child)
{}

unique_ptr<Expression> UnboundSysFuncExpr::copy() const
{
  if (third_child_) {
    return make_unique<UnboundSysFuncExpr>(func_name_.c_str(), child_->copy().release(), second_child_->copy().release(), third_child_->copy().release());
  } else if (second_child_) {
    return make_unique<UnboundSysFuncExpr>(func_name_.c_str(), child_->copy().release(), second_child_->copy().release());
  } else {
    return make_unique<UnboundSysFuncExpr>(func_name_.c_str(), child_->copy().release());
  }
}

unique_ptr<Expression> UnboundSysFuncExpr::deep_copy()
{
  unique_ptr<Expression> child_copy = child_ ? child_->deep_copy() : nullptr;
  unique_ptr<Expression> second_copy = second_child_ ? second_child_->deep_copy() : nullptr;
  unique_ptr<Expression> third_copy = third_child_ ? third_child_->deep_copy() : nullptr;
  if (third_copy) {
    return unique_ptr<Expression>(new UnboundSysFuncExpr(func_name_.c_str(), child_copy.release(), second_copy.release(), third_copy.release()));
  } else if (second_copy) {
    return unique_ptr<Expression>(new UnboundSysFuncExpr(func_name_.c_str(), child_copy.release(), second_copy.release()));
  } else {
    return unique_ptr<Expression>(new UnboundSysFuncExpr(func_name_.c_str(), child_copy.release()));
  }
}

////////////////////////////////////////////////////////////////////////////////

SysFuncExpr::SysFuncExpr(Type type, Expression *child) 
    : sysfunc_type_(type), child_(child), second_child_(nullptr), third_child_(nullptr)
{}

SysFuncExpr::SysFuncExpr(Type type, std::unique_ptr<Expression> child) 
    : sysfunc_type_(type), child_(std::move(child)), second_child_(nullptr), third_child_(nullptr)
{}

SysFuncExpr::SysFuncExpr(Type type, Expression *child, Expression *second_child) 
    : sysfunc_type_(type), child_(child), second_child_(second_child), third_child_(nullptr)
{}

SysFuncExpr::SysFuncExpr(Type type, std::unique_ptr<Expression> child, std::unique_ptr<Expression> second_child) 
    : sysfunc_type_(type), child_(std::move(child)), second_child_(std::move(second_child)), third_child_(nullptr)
{}

SysFuncExpr::SysFuncExpr(Type type, Expression *child, Expression *second_child, Expression *third_child) 
    : sysfunc_type_(type), child_(child), second_child_(second_child), third_child_(third_child)
{}

SysFuncExpr::SysFuncExpr(Type type, std::unique_ptr<Expression> child, std::unique_ptr<Expression> second_child, std::unique_ptr<Expression> third_child) 
    : sysfunc_type_(type), child_(std::move(child)), second_child_(std::move(second_child)), third_child_(std::move(third_child))
{}

bool SysFuncExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const SysFuncExpr &other_func_expr = static_cast<const SysFuncExpr &>(other);
  bool equal = sysfunc_type_ == other_func_expr.sysfunc_type() && child_->equal(*other_func_expr.child());
  if (sysfunc_type_ == Type::DATE_FORMAT || sysfunc_type_ == Type::TOKENIZE || sysfunc_type_ == Type::MATCH_AGAINST) {
    equal = equal && second_child_ && other_func_expr.second_child() && 
            second_child_->equal(*other_func_expr.second_child());
  } else if (sysfunc_type_ == Type::DISTANCE) {
    equal = equal && second_child_ && other_func_expr.second_child() && 
            second_child_->equal(*other_func_expr.second_child()) &&
            third_child_ && other_func_expr.third_child() &&
            third_child_->equal(*other_func_expr.third_child());
  }
  return equal;
}

AttrType SysFuncExpr::value_type() const
{
  switch (sysfunc_type_) {
    case Type::LENGTH:
      return AttrType::INTS;
    case Type::ROUND:
      return AttrType::FLOATS;
    case Type::DATE_FORMAT:
      return AttrType::CHARS;
    case Type::DISTANCE:
      return AttrType::FLOATS;
    case Type::VECTOR_TO_STRING:
      return AttrType::CHARS;
    case Type::STRING_TO_VECTOR:
      return AttrType::VECTORS;
    case Type::TOKENIZE:
      return AttrType::CHARS;
    case Type::MATCH_AGAINST:
      return AttrType::FLOATS;
    default:
      return AttrType::UNDEFINED;
  }
}

int SysFuncExpr::value_length() const
{
  switch (sysfunc_type_) {
    case Type::LENGTH:
      return 4;
    case Type::ROUND:
      return 4;
    case Type::DATE_FORMAT:
      return -1;  // variable length
    case Type::DISTANCE:
      return 4;
    case Type::VECTOR_TO_STRING:
      return -1;  // variable length
    case Type::STRING_TO_VECTOR:
      return -1;  // variable length (depends on vector dimension)
    case Type::TOKENIZE:
      return -1;  // variable length
    case Type::MATCH_AGAINST:
      return 4;  // float
    default:
      return -1;
  }
}

RC SysFuncExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  Value arg_value;
  Value format_value;

  // Get the first argument
  rc = child_->get_value(tuple, arg_value);
  // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
  if (rc == RC::NULL_TUPLE) {
    arg_value.set_null();
  } else if (OB_FAIL(rc)) {
    return rc;
  }

  // Check for NULL
  if (arg_value.attr_type() == AttrType::NULLS) {
    value.set_null();
    return RC::SUCCESS;
  }

  // Get the second argument for functions that need it
  if (sysfunc_type_ == Type::DATE_FORMAT || sysfunc_type_ == Type::DISTANCE || 
      sysfunc_type_ == Type::TOKENIZE || sysfunc_type_ == Type::MATCH_AGAINST) {
    if (!second_child_) {
      LOG_WARN("Function requires second argument");
      return RC::INVALID_ARGUMENT;
    }
    rc = second_child_->get_value(tuple, format_value);
    // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
    if (rc == RC::NULL_TUPLE) {
      format_value.set_null();
    } else if (OB_FAIL(rc)) {
      return rc;
    }
    if (format_value.attr_type() == AttrType::NULLS) {
      value.set_null();
      return RC::SUCCESS;
    }
  }

  // For DISTANCE, we need a third argument (metric type)
  Value metric_value;
  if (sysfunc_type_ == Type::DISTANCE) {
    if (!third_child_) {
      LOG_WARN("DISTANCE function requires third argument (metric type)");
      return RC::INVALID_ARGUMENT;
    }
    rc = third_child_->get_value(tuple, metric_value);
    // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
    if (rc == RC::NULL_TUPLE) {
      metric_value.set_null();
    } else if (OB_FAIL(rc)) {
      return rc;
    }
    if (metric_value.attr_type() == AttrType::NULLS) {
      value.set_null();
      return RC::SUCCESS;
    }
  }

  // Evaluate the function
  switch (sysfunc_type_) {
    case Type::LENGTH:
      return eval_length(arg_value, value);
    case Type::ROUND:
      return eval_round(arg_value, value);
    case Type::DATE_FORMAT:
      return eval_date_format(arg_value, format_value, value);
    case Type::DISTANCE:
      return eval_distance(arg_value, format_value, metric_value, value);
    case Type::VECTOR_TO_STRING:
      return eval_vector_to_string(arg_value, value);
    case Type::STRING_TO_VECTOR:
      return eval_string_to_vector(arg_value, value);
    case Type::TOKENIZE:
      return eval_tokenize(arg_value, format_value, value);
    case Type::MATCH_AGAINST:
      return eval_match_against(arg_value, format_value, value, tuple);
    default:
      return RC::UNIMPLEMENTED;
  }
}

RC SysFuncExpr::try_get_value(Value &value) const
{
  // For try_get_value, we need an empty tuple since we can't evaluate with actual data
  // This is used during parsing/compilation phase
  // For functions that can be evaluated at compile time, try to evaluate them
  // For others, return UNIMPLEMENTED
  
  Value arg_value;
  
  // Try to get the first argument value
  RC rc = child_->try_get_value(arg_value);
  if (OB_FAIL(rc)) {
    return rc;
  }
  
  // Check for NULL
  if (arg_value.attr_type() == AttrType::NULLS) {
    value.set_null();
    return RC::SUCCESS;
  }
  
  // Get the second argument for functions that need it
  Value format_value;
  if (sysfunc_type_ == Type::DATE_FORMAT || sysfunc_type_ == Type::DISTANCE || 
      sysfunc_type_ == Type::TOKENIZE || sysfunc_type_ == Type::MATCH_AGAINST) {
    if (!second_child_) {
      return RC::INVALID_ARGUMENT;
    }
    rc = second_child_->try_get_value(format_value);
    if (OB_FAIL(rc)) {
      return rc;
    }
    if (format_value.attr_type() == AttrType::NULLS) {
      value.set_null();
      return RC::SUCCESS;
    }
  }
  
  // For DISTANCE, we need a third argument (metric type)
  Value metric_value;
  if (sysfunc_type_ == Type::DISTANCE) {
    if (!third_child_) {
      return RC::INVALID_ARGUMENT;
    }
    rc = third_child_->try_get_value(metric_value);
    if (OB_FAIL(rc)) {
      return rc;
    }
    if (metric_value.attr_type() == AttrType::NULLS) {
      value.set_null();
      return RC::SUCCESS;
    }
  }
  
  // Evaluate the function
  switch (sysfunc_type_) {
    case Type::LENGTH:
      return eval_length(arg_value, value);
    case Type::ROUND:
      return eval_round(arg_value, value);
    case Type::DATE_FORMAT:
      return eval_date_format(arg_value, format_value, value);
    case Type::DISTANCE:
      return eval_distance(arg_value, format_value, metric_value, value);
    case Type::VECTOR_TO_STRING:
      return eval_vector_to_string(arg_value, value);
    case Type::STRING_TO_VECTOR:
      return eval_string_to_vector(arg_value, value);
    case Type::TOKENIZE:
      return eval_tokenize(arg_value, format_value, value);
    case Type::MATCH_AGAINST:
      // MATCH_AGAINST requires tuple, so can't be evaluated at compile time
      return RC::UNIMPLEMENTED;
    default:
      return RC::UNIMPLEMENTED;
  }
}

RC SysFuncExpr::eval_length(const Value &arg_value, Value &result) const
{
  // 根据题目要求，LENGTH 只支持 CHAR 类型
  if (arg_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("LENGTH function only supports CHAR type");
    return RC::INVALID_ARGUMENT;
  }

  const char *str = arg_value.data();
  if (str == nullptr) {
    result = Value(0);
    return RC::SUCCESS;
  }

  // Find actual string length (excluding padding)
  int len = arg_value.length();
  if (len > 0) {
    // Remove trailing spaces
    while (len > 0 && str[len - 1] == ' ' ) {
      len--;
    }
  }

  result = Value(len);
  return RC::SUCCESS;
}
RC SysFuncExpr::eval_round(const Value &arg_value, Value &result) const
{
  // 根据题目要求，ROUND 只支持 FLOAT 类型
  if (arg_value.attr_type() != AttrType::FLOATS) {
    LOG_WARN("ROUND function only supports FLOAT type");
    return RC::INVALID_ARGUMENT;
  }

  float val = arg_value.get_float();
  result = Value(static_cast<float>(::round(val)));
  return RC::SUCCESS;
}
RC SysFuncExpr::eval_distance(const Value &v1_value, const Value &v2_value, const Value &metric_value, Value &result) const
{
  if (v1_value.attr_type() != AttrType::VECTORS || v2_value.attr_type() != AttrType::VECTORS) {
    LOG_WARN("DISTANCE function requires VECTOR types");
    return RC::INVALID_ARGUMENT;
  }

  if (metric_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("DISTANCE metric must be a string");
    return RC::INVALID_ARGUMENT;
  }

  const char *metric_str = metric_value.data();
  if (metric_str == nullptr) {
    LOG_WARN("Invalid metric string");
    return RC::INVALID_ARGUMENT;
  }

  // Parse metric type: 'COSINE', 'EUCLIDEAN', 'DOT'
  if (0 == strcasecmp(metric_str, "COSINE")) {
    return Value::cosine_distance(v1_value, v2_value, result);
  } else if (0 == strcasecmp(metric_str, "EUCLIDEAN")) {
    return Value::l2_distance(v1_value, v2_value, result);
  } else if (0 == strcasecmp(metric_str, "DOT")) {
    return Value::inner_product(v1_value, v2_value, result);
  } else {
    LOG_WARN("Invalid distance metric: %s. Must be COSINE, EUCLIDEAN, or DOT", metric_str);
    return RC::INVALID_ARGUMENT;
  }
}

RC SysFuncExpr::eval_vector_to_string(const Value &arg_value, Value &result) const
{
  if (arg_value.attr_type() != AttrType::VECTORS) {
    LOG_WARN("VECTOR_TO_STRING function only supports VECTOR type");
    return RC::INVALID_ARGUMENT;
  }

  vector<float> *vec = arg_value.get_vector();
  if (vec == nullptr || vec->empty()) {
    result = Value("[]", 2);
    return RC::SUCCESS;
  }

  // Format vector as [v1, v2, v3, ...]
  // Always use scientific notation with 5 decimal places, as per MySQL format
  // Example: [3.07000e+00,-1.24000e+00]
  string result_str = "[";
  for (size_t i = 0; i < vec->size(); i++) {
    if (i > 0) {
      result_str += ",";
    }
    
    // Format float with scientific notation, 5 decimal places
    float val = vec->at(i);
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.5e", static_cast<double>(val));
    result_str += buffer;
  }
  result_str += "]";

  result = Value(result_str.c_str(), result_str.length());
  return RC::SUCCESS;
}

RC SysFuncExpr::eval_string_to_vector(const Value &arg_value, Value &result) const
{
  if (arg_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("STRING_TO_VECTOR function only supports CHAR type");
    return RC::INVALID_ARGUMENT;
  }

  const char *str = arg_value.data();
  if (str == nullptr) {
    LOG_WARN("Invalid string for STRING_TO_VECTOR");
    return RC::INVALID_ARGUMENT;
  }

  // Parse string like "[1, 2, 3]" or "[1.5, 2.5, 3.5]"
  int len = arg_value.length();
  if (len < 2 || str[0] != '[' || str[len - 1] != ']') {
    LOG_WARN("STRING_TO_VECTOR: string must be in format [v1, v2, v3, ...]");
    return RC::INVALID_ARGUMENT;
  }

  // Extract the content between brackets
  string content(str + 1, len - 2);  // Skip '[' and ']'
  
  // Parse comma-separated values
  vector<float> vec;
  stringstream ss(content);
  string item;
  
  while (getline(ss, item, ',')) {
    // Trim whitespace
    item.erase(0, item.find_first_not_of(" \t"));
    item.erase(item.find_last_not_of(" \t") + 1);
    
    if (!item.empty()) {
      try {
        float val = std::stof(item);
        vec.push_back(val);
      } catch (const std::exception &e) {
        LOG_WARN("STRING_TO_VECTOR: failed to parse value: %s", item.c_str());
        return RC::INVALID_ARGUMENT;
      }
    }
  }

  if (vec.empty()) {
    LOG_WARN("STRING_TO_VECTOR: empty vector");
    return RC::INVALID_ARGUMENT;
  }

  result.set_vector(std::move(vec));
  return RC::SUCCESS;
}

RC SysFuncExpr::eval_tokenize(const Value &text_value, const Value &parser_value, Value &result) const
{
  if (text_value.attr_type() != AttrType::CHARS && text_value.attr_type() != AttrType::TEXTS) {
    LOG_WARN("TOKENIZE function only supports CHAR or TEXT type");
    return RC::INVALID_ARGUMENT;
  }

  if (parser_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("TOKENIZE parser must be CHAR type");
    return RC::INVALID_ARGUMENT;
  }

  const char *text = text_value.data();
  const char *parser_str = parser_value.data();
  
  if (text == nullptr || parser_str == nullptr) {
    result = Value("", 0);
    return RC::SUCCESS;
  }

  string text_str(text);
  string parser(parser_str);
  
  // Convert parser to lowercase for comparison
  transform(parser.begin(), parser.end(), parser.begin(), ::tolower);
  
  vector<string> tokens;
  
  if (parser == "jieba") {
    // Use jieba tokenization (simplified implementation)
    // In production, this should call cppjieba library
    tokens = tokenize_jieba(text_str);
  } else {
    // Default: space-based tokenization
    istringstream iss(text_str);
    string word;
    while (iss >> word) {
      // Remove punctuation
      string cleaned;
      for (char c : word) {
        if (isalnum(c) || c == '_') {
          cleaned += c;
        }
      }
      if (!cleaned.empty()) {
        tokens.push_back(cleaned);
      }
    }
  }
  
  // Format tokens as JSON array: ["token1", "token2", ...]
  string result_str = "[";
  for (size_t i = 0; i < tokens.size(); i++) {
    if (i > 0) result_str += ", ";
    result_str += "\"";
    // Escape quotes in token
    for (char c : tokens[i]) {
      if (c == '"' || c == '\\') {
        result_str += '\\';
      }
      result_str += c;
    }
    result_str += "\"";
  }
  result_str += "]";
  
  result = Value(result_str.c_str(), result_str.length());
  return RC::SUCCESS;
}

RC SysFuncExpr::eval_match_against(const Value &field_value, const Value &query_value, Value &result, const Tuple &tuple) const
{
  if (field_value.attr_type() != AttrType::CHARS && field_value.attr_type() != AttrType::TEXTS) {
    LOG_WARN("MATCH AGAINST function only supports CHAR or TEXT type");
    return RC::INVALID_ARGUMENT;
  }

  if (query_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("MATCH AGAINST query must be CHAR type");
    return RC::INVALID_ARGUMENT;
  }

  const char *query_text = query_value.data();
  if (query_text == nullptr) {
    result = Value(0.0f);
    return RC::SUCCESS;
  }

  string query_str(query_text);

  // Try to get fulltext index from FieldExpr and tuple
  if (child_ && child_->type() == ExprType::FIELD) {
    const FieldExpr *field_expr = static_cast<const FieldExpr *>(child_.get());
    const BaseTable *table = field_expr->table();
    const char *field_name = field_expr->field_name();
    
    if (table && field_name && !table->is_view()) {
      // Try to get RID from tuple
      const BaseTable *tuple_table = nullptr;
      RID rid;
      RC rc = tuple.get_tuple_rid(0, tuple_table, rid);
      
      if (rc == RC::SUCCESS && tuple_table == table) {
        // Find fulltext index for this field
        const Table *real_table = static_cast<const Table *>(table);
        Index *index = real_table->find_index_by_field(field_name);
        
        if (index && index->is_fulltext_index()) {
          FullTextIndex *fulltext_index = static_cast<FullTextIndex *>(index);
          double bm25_score = fulltext_index->calculate_bm25_score(query_str, rid);
          result = Value(static_cast<float>(bm25_score));
          return RC::SUCCESS;
        }
      }
    }
  }

  // Fallback: simple word frequency scoring
  const char *field_text = field_value.data();
  if (field_text == nullptr) {
    result = Value(0.0f);
    return RC::SUCCESS;
  }

  string field_str(field_text);
  
  // Simple scoring: count how many query words appear in field
  istringstream query_iss(query_str);
  string query_word;
  int match_count = 0;
  
  while (query_iss >> query_word) {
    // Convert to lowercase for comparison
    transform(query_word.begin(), query_word.end(), query_word.begin(), ::tolower);
    transform(field_str.begin(), field_str.end(), field_str.begin(), ::tolower);
    
    if (field_str.find(query_word) != string::npos) {
      match_count++;
    }
  }
  
  // Return simple score
  result = Value(static_cast<float>(match_count));
  return RC::SUCCESS;
}

// Helper function for jieba tokenization (simplified)
vector<string> SysFuncExpr::tokenize_jieba(const string &text) const
{
  vector<string> tokens;
  
  // Simplified implementation: split by common Chinese punctuation and spaces
  // In production, this should use cppjieba library
  string current_token;
  // Chinese punctuation marks (UTF-8 encoded as strings)
  const string chinese_punct = "，。！？；：、";
  
  for (size_t i = 0; i < text.length(); ) {
    unsigned char c = static_cast<unsigned char>(text[i]);
    
    // Determine UTF-8 character length
    size_t char_len = 1;
    if ((c & 0x80) != 0) {
      if ((c & 0xE0) == 0xC0) {
        char_len = 2;  // 2-byte UTF-8
      } else if ((c & 0xF0) == 0xE0) {
        char_len = 3;  // 3-byte UTF-8 (Chinese characters)
      } else if ((c & 0xF8) == 0xF0) {
        char_len = 4;  // 4-byte UTF-8
      }
    }
    
    // Extract the complete UTF-8 character
    string char_str = text.substr(i, char_len);
    
    // Check if it's punctuation (space or Chinese punctuation)
    bool is_punct = false;
    if (isspace(c)) {
      is_punct = true;
    } else if (char_len > 1 && chinese_punct.find(char_str) != string::npos) {
      is_punct = true;
    }
    
    if (is_punct) {
      if (!current_token.empty()) {
        tokens.push_back(current_token);
        current_token.clear();
      }
    } else {
      current_token += char_str;
    }
    
    i += char_len;
  }
  
  if (!current_token.empty()) {
    tokens.push_back(current_token);
  }
  
  return tokens;
}

RC SysFuncExpr::eval_date_format(const Value &date_value, const Value &format_value, Value &result) const
{
  if (date_value.attr_type() != AttrType::DATES) {
    LOG_WARN("DATE_FORMAT function only supports DATE type");
    return RC::INVALID_ARGUMENT;
  }

  if (format_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("DATE_FORMAT format string must be CHAR type");
    return RC::INVALID_ARGUMENT;
  }

  const char *date_str = date_value.data();
  const char *format_str = format_value.data();

  if (date_str == nullptr || format_str == nullptr) {
    LOG_WARN("Invalid date or format string");
    return RC::INVALID_ARGUMENT;
  }

  // Parse date string (format: YYYY-MM-DD, length is 10)
  if (strlen(date_str) != 10) {
    LOG_WARN("Invalid date format length: %s", date_str);
    return RC::INVALID_ARGUMENT;
  }

  int year, month, day;
  if (sscanf(date_str, "%4d-%2d-%2d", &year, &month, &day) != 3) {
    LOG_WARN("Invalid date format: %s", date_str);
    return RC::INVALID_ARGUMENT;
  }

  // Month names
  static const char *month_names[] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
  };

  // Day suffixes
  static const char *day_suffix[] = {
    "th", "st", "nd", "rd", "th", "th", "th", "th", "th", "th",
    "th", "th", "th", "th", "th", "th", "th", "th", "th", "th",
    "th", "st", "nd", "rd", "th", "th", "th", "th", "th", "th", "th"
  };

  // Build formatted string
  string result_str;
  const char *p = format_str;
  while (*p != '\0') {
    if (*p == '%' && *(p + 1) != '\0') {
      p++;
      switch (*p) {
        case 'Y':  // 4-digit year
          result_str += std::to_string(year);
          break;
        case 'y':  // 2-digit year
          result_str += (year % 100 < 10 ? "0" : "") + std::to_string(year % 100);
          break;
        case 'm':  // Month (01-12)
          result_str += (month < 10 ? "0" : "") + std::to_string(month);
          break;
        case 'd':  // Day of month (01-31)
          result_str += (day < 10 ? "0" : "") + std::to_string(day);
          break;
        case 'D':  // Day of month with suffix
          result_str += std::to_string(day) + day_suffix[day % 31];
          break;
        case 'M':  // Month name
          if (month >= 1 && month <= 12) {
            result_str += month_names[month];
          }
          break;
        default:
          // Invalid format specifier, output as-is
          result_str += '%';
          result_str += *p;
          break;
      }
      p++;
    } else {
      result_str += *p;
      p++;
    }
  }

  result = Value(result_str.c_str(), result_str.length());
  return RC::SUCCESS;
}

RC SysFuncExpr::type_from_string(const char *type_str, SysFuncExpr::Type &func_type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "length")) {
    func_type = Type::LENGTH;
  } else if (0 == strcasecmp(type_str, "round")) {
    func_type = Type::ROUND;
  } else if (0 == strcasecmp(type_str, "date_format")) {
    func_type = Type::DATE_FORMAT;
  } else if (0 == strcasecmp(type_str, "distance")) {
    func_type = Type::DISTANCE;
  } else if (0 == strcasecmp(type_str, "vector_to_string")) {
    func_type = Type::VECTOR_TO_STRING;
  } else if (0 == strcasecmp(type_str, "string_to_vector")) {
    func_type = Type::STRING_TO_VECTOR;
  } else if (0 == strcasecmp(type_str, "tokenize")) {
    func_type = Type::TOKENIZE;
  } else if (0 == strcasecmp(type_str, "match_against")) {
    func_type = Type::MATCH_AGAINST;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

unique_ptr<Expression> SysFuncExpr::copy() const
{
  if (third_child_) {
    return make_unique<SysFuncExpr>(sysfunc_type_, child_->copy(), second_child_->copy(), third_child_->copy());
  } else if (second_child_) {
    return make_unique<SysFuncExpr>(sysfunc_type_, child_->copy(), second_child_->copy());
  } else {
    return make_unique<SysFuncExpr>(sysfunc_type_, child_->copy());
  }
}

unique_ptr<Expression> SysFuncExpr::deep_copy()
{
  unique_ptr<Expression> child_copy = child_ ? child_->deep_copy() : nullptr;
  unique_ptr<Expression> second_copy = second_child_ ? second_child_->deep_copy() : nullptr;
  unique_ptr<Expression> third_copy = third_child_ ? third_child_->deep_copy() : nullptr;
  if (third_copy) {
    return unique_ptr<Expression>(new SysFuncExpr(sysfunc_type_, std::move(child_copy), std::move(second_copy), std::move(third_copy)));
  } else if (second_copy) {
    return unique_ptr<Expression>(new SysFuncExpr(sysfunc_type_, std::move(child_copy), std::move(second_copy)));
  } else {
    return unique_ptr<Expression>(new SysFuncExpr(sysfunc_type_, std::move(child_copy)));
  }
}
