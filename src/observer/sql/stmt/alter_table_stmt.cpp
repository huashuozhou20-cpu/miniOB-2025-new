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

#include "sql/stmt/alter_table_stmt.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/lang/string.h"
RC AlterTableStmt::create(Db *db, AlterTableSqlNode &alter_sql, Stmt *&stmt)
{
  stmt = nullptr;

  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  const char *table_name = alter_sql.relation_name.c_str();
  if (common::is_blank(table_name)) {
    LOG_WARN("invalid argument. table name is blank");
    return RC::INVALID_ARGUMENT;
  }

  // 检查表是否存在
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 验证操作
  switch (alter_sql.alter_type) {
    case AlterTableSqlNode::AlterType::ADD_COLUMN: {
      // 检查列名是否已存在
      const FieldMeta *field = table->table_meta().field(alter_sql.attr_info.name.c_str());
      if (field != nullptr) {
        LOG_WARN("column already exists. table=%s, column=%s", table_name, alter_sql.attr_info.name.c_str());
        return RC::SCHEMA_TABLE_EXIST;
      }
      break;
    }
    case AlterTableSqlNode::AlterType::DROP_COLUMN: {
      // 检查列是否存在
      const FieldMeta *field = table->table_meta().field(alter_sql.old_name.c_str());
      if (field == nullptr) {
        LOG_WARN("column does not exist. table=%s, column=%s", table_name, alter_sql.old_name.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      // 不能删除系统字段
      if (field->field_id() < table->table_meta().sys_field_num()) {
        LOG_WARN("cannot drop system field. table=%s, column=%s", table_name, alter_sql.old_name.c_str());
        return RC::INVALID_ARGUMENT;
      }
      break;
    }
    case AlterTableSqlNode::AlterType::RENAME_COLUMN: {
      // 检查旧列是否存在
      const FieldMeta *old_field = table->table_meta().field(alter_sql.old_name.c_str());
      if (old_field == nullptr) {
        LOG_WARN("column does not exist. table=%s, column=%s", table_name, alter_sql.old_name.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      // 检查新列名是否已存在
      const FieldMeta *new_field = table->table_meta().field(alter_sql.new_name.c_str());
      if (new_field != nullptr) {
        LOG_WARN("column already exists. table=%s, column=%s", table_name, alter_sql.new_name.c_str());
        return RC::SCHEMA_TABLE_EXIST;
      }
      break;
    }
    case AlterTableSqlNode::AlterType::RENAME_TABLE: {
      // 检查新表名是否已存在
      Table *new_table = db->find_table(alter_sql.new_name.c_str());
      if (new_table != nullptr) {
        LOG_WARN("table already exists. db=%s, table_name=%s", db->name(), alter_sql.new_name.c_str());
        return RC::SCHEMA_TABLE_EXIST;
      }
      break;
    }
    default: {
      LOG_WARN("unknown alter type");
      return RC::INVALID_ARGUMENT;
    }
  }

  stmt = new AlterTableStmt(table, alter_sql);
  return RC::SUCCESS;
}

