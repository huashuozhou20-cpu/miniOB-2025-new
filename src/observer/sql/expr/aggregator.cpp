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
// Created by Wangyunlai on 2024/05/29.
//

#include "sql/expr/aggregator.h"
#include "common/log/log.h"
#include "aggregator.h"

RC SumAggregator::accumulate(const Value &value)
{
  if(value.attr_type() == AttrType::NULLS)return RC::SUCCESS;
  else if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }
  
  // 类型检查：如果类型不匹配，尝试类型转换
  if (value.attr_type() != value_.attr_type()) {
    // 如果 value_ 是 float，尝试将 value 转换为 float
    if (value_.attr_type() == AttrType::FLOATS && value.attr_type() == AttrType::INTS) {
      Value float_value;
      RC rc = Value::cast_to(value, AttrType::FLOATS, float_value);
      if (rc == RC::SUCCESS) {
        Value::add(float_value, value_, value_);
        return RC::SUCCESS;
      }
    }
    // 如果 value_ 是 int，尝试将 value 转换为 int
    if (value_.attr_type() == AttrType::INTS && value.attr_type() == AttrType::FLOATS) {
      // 如果第一个值是 int，后续出现 float，应该统一转换为 float
      Value float_value_;
      RC rc = Value::cast_to(value_, AttrType::FLOATS, float_value_);
      if (rc == RC::SUCCESS) {
        value_ = float_value_;
        Value::add(value, value_, value_);
        return RC::SUCCESS;
      }
    }
    LOG_WARN("type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    ASSERT(false, "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    return RC::INVALID_ARGUMENT;
  }
  
  Value::add(value, value_, value_);
  return RC::SUCCESS;
}

RC SumAggregator::evaluate(Value& result, bool have_groub_by)
{
  if (value_.attr_type() == AttrType::UNDEFINED)
    result.set_null();
  else result = std::move(value_);
  return RC::SUCCESS;
}

RC MaxAggregator::accumulate(const Value &value)
{
  if(value.attr_type() == AttrType::NULLS)return RC::SUCCESS;
  else if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }
  
  // MAX 需要类型兼容，但允许 int 和 float 之间的比较
  if (value.attr_type() == value_.attr_type()) {
    Value::max(std::move(value), std::move(value_), value_);
  } else if ((value_.attr_type() == AttrType::INTS && value.attr_type() == AttrType::FLOATS) ||
             (value_.attr_type() == AttrType::FLOATS && value.attr_type() == AttrType::INTS)) {
    // int 和 float 可以比较，统一转换为 float
    Value float_value;
    Value float_value_;
    RC rc1 = Value::cast_to(value, AttrType::FLOATS, float_value);
    RC rc2 = Value::cast_to(value_, AttrType::FLOATS, float_value_);
    if (rc1 == RC::SUCCESS && rc2 == RC::SUCCESS) {
      Value::max(std::move(float_value), std::move(float_value_), value_);
    } else {
      LOG_WARN("failed to cast value for MAX aggregation");
      return RC::INVALID_ARGUMENT;
    }
  } else {
    LOG_WARN("type mismatch for MAX aggregation. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    ASSERT(false, "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    return RC::INVALID_ARGUMENT;
  }
  
  return RC::SUCCESS;
}

RC MaxAggregator::evaluate(Value& result, bool have_groub_by)
{
  if (value_.attr_type() == AttrType::UNDEFINED)
    result.set_null();
  else result = std::move(value_);
  return RC::SUCCESS;
}

RC MinAggregator::accumulate(const Value &value)
{
  if(value.attr_type() == AttrType::NULLS)return RC::SUCCESS;
  else if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }
  
  // MIN 需要类型兼容，但允许 int 和 float 之间的比较
  if (value.attr_type() == value_.attr_type()) {
    Value::min(std::move(value), std::move(value_), value_);
  } else if ((value_.attr_type() == AttrType::INTS && value.attr_type() == AttrType::FLOATS) ||
             (value_.attr_type() == AttrType::FLOATS && value.attr_type() == AttrType::INTS)) {
    // int 和 float 可以比较，统一转换为 float
    Value float_value;
    Value float_value_;
    RC rc1 = Value::cast_to(value, AttrType::FLOATS, float_value);
    RC rc2 = Value::cast_to(value_, AttrType::FLOATS, float_value_);
    if (rc1 == RC::SUCCESS && rc2 == RC::SUCCESS) {
      Value::min(std::move(float_value), std::move(float_value_), value_);
    } else {
      LOG_WARN("failed to cast value for MIN aggregation");
      return RC::INVALID_ARGUMENT;
    }
  } else {
    LOG_WARN("type mismatch for MIN aggregation. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    ASSERT(false, "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
    return RC::INVALID_ARGUMENT;
  }
  
  return RC::SUCCESS;
}

RC MinAggregator::evaluate(Value& result, bool have_groub_by)
{
  if (value_.attr_type() == AttrType::UNDEFINED)
    result.set_null();
  else result = std::move(value_);
  return RC::SUCCESS;
}

RC AvgAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    // 初始化时，根据输入值的类型来设置 value_ 的类型（如果不是 NULL）
    if(value.attr_type() == AttrType::NULLS) {
      // 如果第一个值就是 NULL，暂时设置为 float 类型（avg 总是返回 float）
      value_ = Value((float)0.0);
    } else {
      // 对于非 NULL 值，先转换为 float 类型
      Value float_value;
      RC rc = Value::cast_to(value, AttrType::FLOATS, float_value);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to cast value to float for avg aggregation");
        return rc;
      }
      value_ = float_value;
    }
    countnum = Value((int)0);
  }
  if(value.attr_type() == AttrType::NULLS)return RC::SUCCESS;
  
  // 对于非 NULL 值，需要转换为 float 类型
  Value float_value;
  RC rc = Value::cast_to(value, AttrType::FLOATS, float_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to cast value to float for avg aggregation");
    return rc;
  }
  
  Value::avg(float_value, value_, countnum);
  return RC::SUCCESS;
}

RC AvgAggregator::evaluate(Value& result, bool have_groub_by)
{
  if (value_.attr_type() == AttrType::UNDEFINED)
    result.set_null();
  else {
    result.set_type(AttrType::FLOATS);
    Value::divide(value_, countnum, result);
  }
  return RC::SUCCESS;
}

RC CountAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = Value((int)0);
  }
  // COUNT 忽略 NULL 值，不计数
  if(value.attr_type() == AttrType::NULLS)return RC::SUCCESS;

  // COUNT 不关心值的类型，只要不是 NULL 就计数
  // 移除类型检查，因为 COUNT 可以处理任何类型的值
  Value::count(value_);
  return RC::SUCCESS;
}

RC CountAggregator::evaluate(Value& result, bool have_groub_by)
{
  if (value_.attr_type() == AttrType::UNDEFINED){
    if(have_groub_by)result.set_null();
    else result = Value((int)0);
  }
  else result = std::move(value_);
  return RC::SUCCESS;
}
