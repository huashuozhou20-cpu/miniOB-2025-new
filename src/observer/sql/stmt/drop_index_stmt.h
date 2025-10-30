#pragma once
#include "sql/stmt/stmt.h"
#include <string>

class Db;
class Table;

class DropIndexStmt : public Stmt {
public:
  static RC create(Db *db, const DropIndexSqlNode &node, Stmt *&stmt);

  StmtType type() const override { return StmtType::DROP_INDEX; } // 你的枚举名可能不同
  Table *table() const { return table_; }
  const std::string &index_name() const { return index_name_; }
private:
  Table *table_ = nullptr;
  std::string index_name_;
};
