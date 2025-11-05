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
// Created on 2024/9/10.
//

#pragma once

#include <string>
#include <vector>

#include "sql/stmt/stmt.h"

class Db;

/**
 * @brief 表示删除索引的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class DropIndexStmt : public Stmt
{
public:
  DropIndexStmt(
      const std::string &table_name,
      const std::string &index_name)
      : table_name_(table_name), index_name_(index_name)
  {}
  virtual ~DropIndexStmt() = default;

  StmtType type() const override { return StmtType::DROP_INDEX; }

  const std::string &table_name() const { return table_name_; }
  const std::string &index_name() const { return index_name_; }

  static RC create(Db *db, const DropIndexSqlNode &drop_index, Stmt *&stmt);

private:
  std::string table_name_;
  std::string index_name_;
};

