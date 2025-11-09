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
// Created by Meiyi
//

#include "sql/parser/parse.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

static inline std::string to_lower_copy(const std::string &s)
{
  std::string r(s);
  std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return r;
}

static inline void split_tokens(const std::string &s, std::vector<std::string> &out)
{
  out.clear();
  std::string cur;
  for (size_t i = 0; i < s.size(); i++) {
    char c = s[i];
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!cur.empty()) {
        out.emplace_back(cur);
        cur.clear();
      }
      continue;
    }
    cur.push_back(c);
  }
  if (!cur.empty()) {
    out.emplace_back(cur);
  }
}

static bool parse_attr_def_tokens(const std::vector<std::string> &toks, size_t start_idx, AttrInfoSqlNode &attr_info, size_t &consumed)
{
  // Expect: <col_name> <type> [ ( <len> ) ] [ NOT NULL | NULL ]
  consumed = 0;
  if (start_idx + 2 > toks.size()) {
    return false;
  }
  const std::string &col = toks[start_idx];
  const std::string type_raw = to_lower_copy(toks[start_idx + 1]);

  attr_info.name     = col;
  attr_info.nullable = true;
  if (type_raw == "int") {
    attr_info.type   = AttrType::INTS;
    attr_info.length = 4;
    consumed         = 2;
  } else if (type_raw.rfind("char", 0) == 0) {
    // handle "char" and possibly next tokens like "(10)"
    attr_info.type = AttrType::CHARS;
    attr_info.length = 1; // default minimal
    consumed = 2;
    // check next token if looks like "(n)"
    if (start_idx + 2 < toks.size()) {
      const std::string &next = toks[start_idx + 2];
      if (!next.empty() && next.front() == '(' && next.back() == ')' && next.size() > 2) {
        attr_info.length = static_cast<size_t>(std::max(1, atoi(next.substr(1, next.size() - 2).c_str())));
        consumed = 3;
      }
    }
  } else if (type_raw == "float") {
    attr_info.type   = AttrType::FLOATS;
    attr_info.length = 4;
    consumed         = 2;
  } else if (type_raw == "date") {
    attr_info.type   = AttrType::DATES;
    attr_info.length = 10;
    consumed         = 2;
  } else if (type_raw == "vector") {
    attr_info.type   = AttrType::VECTORS;
    attr_info.length = 0;
    consumed         = 2;
  } else if (type_raw == "text") {
    attr_info.type   = AttrType::TEXTS;
    attr_info.length = 0;
    consumed         = 2;
  } else {
    return false;
  }

  // nullable tail [NOT NULL | NULL]
  if (start_idx + consumed < toks.size()) {
    std::string tail = to_lower_copy(toks[start_idx + consumed]);
    if (tail == "not") {
      if (start_idx + consumed + 1 < toks.size() && to_lower_copy(toks[start_idx + consumed + 1]) == "null") {
        attr_info.nullable = false;
        consumed += 2;
      }
    } else if (tail == "null") {
      attr_info.nullable = true;
      consumed += 1;
    }
  }
  return true;
}

static bool fallback_parse_alter(const char *st, ParsedSqlResult *sql_result)
{
  if (st == nullptr) {
    return false;
  }
  std::string sql(st);
  // strip trailing semicolon
  while (!sql.empty() && (sql.back() == ';' || std::isspace(static_cast<unsigned char>(sql.back())))) {
    sql.pop_back();
  }
  std::string lower = to_lower_copy(sql);
  // quick gate
  if (lower.rfind("alter table ", 0) != 0) {
    return false;
  }

  // tokenize
  std::vector<std::string> toks;
  split_tokens(lower, toks);
  // also split original for names preserving case (use simple split)
  std::vector<std::string> toks_raw;
  split_tokens(sql, toks_raw);
  if (toks.size() < 4) {
    return false;
  }
  // toks: [alter, table, <rel>, ...]
  const std::string relation = toks_raw[2];

  size_t i = 3;
  if (i < toks.size() && toks[i] == "add") {
    i += 1;
    if (i < toks.size() && toks[i] == "column") {
      i += 1;
    }
    AttrInfoSqlNode attr;
    size_t consumed = 0;
    if (!parse_attr_def_tokens(toks_raw, i, attr, consumed)) {
      return false;
    }
    auto node = std::make_unique<ParsedSqlNode>(SCF_ALTER_TABLE);
    node->alter_table.relation_name = relation;
    node->alter_table.alter_type    = AlterTableSqlNode::AlterType::ADD_COLUMN;
    node->alter_table.attr_info     = attr;
    sql_result->add_sql_node(std::move(node));
    return true;
  }
  if (i < toks.size() && toks[i] == "drop") {
    i += 1;
    if (i < toks.size() && toks[i] == "column") {
      i += 1;
    }
    if (i >= toks_raw.size()) {
      return false;
    }
    auto node = std::make_unique<ParsedSqlNode>(SCF_ALTER_TABLE);
    node->alter_table.relation_name = relation;
    node->alter_table.alter_type    = AlterTableSqlNode::AlterType::DROP_COLUMN;
    node->alter_table.old_name      = toks_raw[i];
    sql_result->add_sql_node(std::move(node));
    return true;
  }
  if (i < toks.size() && toks[i] == "rename") {
    i += 1;
    if (i < toks.size() && toks[i] == "column") {
      // RENAME COLUMN <old> TO <new>
      i += 1;
      if (i + 2 >= toks_raw.size()) {
        return false;
      }
      const std::string old_name = toks_raw[i];
      if (to_lower_copy(toks[i + 1]) != "to") {
        return false;
      }
      const std::string new_name = toks_raw[i + 2];
      auto node = std::make_unique<ParsedSqlNode>(SCF_ALTER_TABLE);
      node->alter_table.relation_name = relation;
      node->alter_table.alter_type    = AlterTableSqlNode::AlterType::RENAME_COLUMN;
      node->alter_table.old_name      = old_name;
      node->alter_table.new_name      = new_name;
      sql_result->add_sql_node(std::move(node));
      return true;
    } else if (i < toks.size() && toks[i] == "to") {
      // RENAME TO <new>
      i += 1;
      if (i >= toks_raw.size()) {
        return false;
      }
      auto node = std::make_unique<ParsedSqlNode>(SCF_ALTER_TABLE);
      node->alter_table.relation_name = relation;
      node->alter_table.alter_type    = AlterTableSqlNode::AlterType::RENAME_TABLE;
      node->alter_table.new_name      = toks_raw[i];
      sql_result->add_sql_node(std::move(node));
      return true;
    }
    return false;
  }
  if (i < toks.size() && toks[i] == "change") {
    i += 1;
    if (i < toks.size() && toks[i] == "column") {
      i += 1;
    }
    if (i >= toks_raw.size()) {
      return false;
    }
    const std::string old_name = toks_raw[i];
    AttrInfoSqlNode attr;
    size_t consumed = 0;
    if (!parse_attr_def_tokens(toks_raw, i + 1, attr, consumed)) {
      return false;
    }
    auto node = std::make_unique<ParsedSqlNode>(SCF_ALTER_TABLE);
    node->alter_table.relation_name = relation;
    node->alter_table.alter_type    = AlterTableSqlNode::AlterType::CHANGE_COLUMN;
    node->alter_table.old_name      = old_name;
    node->alter_table.attr_info     = attr;
    node->alter_table.new_name      = attr.name;
    sql_result->add_sql_node(std::move(node));
    return true;
  }
  return false;
}

RC parse(char *st, ParsedSqlNode *sqln);

ParsedSqlNode::ParsedSqlNode() : flag(SCF_ERROR) {}

ParsedSqlNode::ParsedSqlNode(SqlCommandFlag _flag) : flag(_flag) {}

void ParsedSqlResult::add_sql_node(unique_ptr<ParsedSqlNode> sql_node)
{
  sql_nodes_.emplace_back(std::move(sql_node));
}

////////////////////////////////////////////////////////////////////////////////

int sql_parse(const char *st, ParsedSqlResult *sql_result);

RC parse(const char *st, ParsedSqlResult *sql_result)
{
  // fast path fallback for ALTER TABLE when bison grammar does not recognize it
  if (fallback_parse_alter(st, sql_result)) {
    return RC::SUCCESS;
  }
  sql_parse(st, sql_result);
  return RC::SUCCESS;
}
