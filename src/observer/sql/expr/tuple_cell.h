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
// Created by WangYunlai on 2022/6/7.
//

#pragma once

#include <iostream>
#include "storage/table/table.h"
#include "storage/field/field_meta.h"
#include <cstring>
#include <string>

using namespace std;
class TupleCell
{
public:
  TupleCell() = default;

  // 设置/访问底层数据与类型（很多地方会用到）
  void set_type(AttrType t) { type_ = t; }
  void set_data(const void *p, int len) { data_ = p; length_ = len; }

  AttrType    attr_type() const { return type_; }
  const void *data()      const { return data_; }
  int         length()    const { return length_; }

  // 这两个就是你在 tuple_cell.cpp 里要实现/已经实现的成员函数
  void to_string(string &out) const;
  int  compare(const TupleCell &other) const;

private:
  AttrType    type_   {(AttrType)0};  // 不假设有 UNDEFINED，安全兜底
  const void *data_   {nullptr};
  int         length_ {0};
};
// ===================== END NEW =====================================

class TupleCellSpec final
{
public:
  TupleCellSpec() = default;
  TupleCellSpec(const char *table_name, const char *field_name, const char *alias = nullptr);
  explicit TupleCellSpec(const char *alias);
  explicit TupleCellSpec(const string &alias);

  const char *table_name() const { return table_name_.c_str(); }
  const char *field_name() const { return field_name_.c_str(); }
  const char *alias() const { return alias_.c_str(); }

  bool equals(const TupleCellSpec &other) const
  {
    return table_name_ == other.table_name_ && field_name_ == other.field_name_ && alias_ == other.alias_;
  }

private:
  string table_name_;
  string field_name_;
  string alias_;
};
