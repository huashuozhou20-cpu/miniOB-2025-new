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

#include "sql/executor/alter_table_executor.h"
#include "common/log/log.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "session/session.h"
#include "sql/stmt/alter_table_stmt.h"
#include "storage/table/table.h"

RC AlterTableExecutor::execute(SQLStageEvent *sql_event)
{
  Stmt    *stmt    = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  
  ASSERT(stmt->type() == StmtType::ALTER_TABLE,
      "alter table executor can not run this command: %d",
      static_cast<int>(stmt->type()));

  AlterTableStmt *alter_table_stmt = static_cast<AlterTableStmt *>(stmt);
  Table *table = alter_table_stmt->table();
  
  Trx *trx = session->current_trx();
  
  int alter_type_int = static_cast<int>(alter_table_stmt->alter_type());
  RC rc = table->alter_table(trx, alter_type_int,
                             alter_table_stmt->attr_info(),
                             alter_table_stmt->old_name(),
                             alter_table_stmt->new_name());
  
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to alter table. table=%s, rc=%s", table->name(), strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

