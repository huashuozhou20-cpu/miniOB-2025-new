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
// Created by Wangyunlai on 2024/12/07.
//

#pragma once

#include <string>
#include "sql/stmt/stmt.h"
#include "sql/parser/parse_defs.h"

class Db;
class Table;

/**
 * @brief 表示alter table语句
 * @ingroup Statement
 */
class AlterTableStmt : public Stmt
{
public:
  AlterTableStmt(Table *table, AlterTableSqlNode &alter_sql)
      : table_(table), alter_type_(alter_sql.alter_type), attr_info_(alter_sql.attr_info),
        old_name_(alter_sql.old_name), new_name_(alter_sql.new_name)
  {}

  virtual ~AlterTableStmt() = default;

  StmtType type() const override { return StmtType::ALTER_TABLE; }

  Table *table() const { return table_; }
  AlterTableSqlNode::AlterType alter_type() const { return alter_type_; }
  const AttrInfoSqlNode &attr_info() const { return attr_info_; }
  const std::string &old_name() const { return old_name_; }
  const std::string &new_name() const { return new_name_; }

public:
  static RC create(Db *db, AlterTableSqlNode &alter_sql, Stmt *&stmt);

private:
  Table *table_ = nullptr;
  AlterTableSqlNode::AlterType alter_type_;
  AttrInfoSqlNode attr_info_;
  std::string old_name_;
  std::string new_name_;
};

