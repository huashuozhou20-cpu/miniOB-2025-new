#include "sql/stmt/drop_index_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/lang/string.h"
#include "common/rc.h"
#include "common/log/log.h"

RC DropIndexStmt::create(Db *db, const DropIndexSqlNode &node, Stmt *&stmt)
{
  stmt = nullptr;
  if (common::is_blank(node.relation_name.c_str()) || common::is_blank(node.index_name.c_str())) {
    return RC::INVALID_ARGUMENT;
  }
  Table *table = db->find_table(node.relation_name.c_str());
  if (nullptr == table) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  if (nullptr == table->find_index(node.index_name.c_str())) {
    // 你可以扩展 RC::SCHEMA_INDEX_NOT_EXIST；没有就用 SCHEMA_FIELD_NOT_EXIST 兜底
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  auto *s = new DropIndexStmt();
  s->table_ = table;
  s->index_name_ = node.index_name;
  stmt = s;
  return RC::SUCCESS;
}
