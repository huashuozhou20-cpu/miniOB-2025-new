/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Longda on 2021/4/13.
//

#include "storage/default/default_handler.h"

#include <string>
#include <filesystem>
#include <unistd.h>
#include <cstring>

#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/os/path.h"
#include "session/session.h"
#include "storage/common/condition_filter.h"
#include "storage/index/bplus_tree.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "storage/db/db.h"

using namespace std;

DefaultHandler::DefaultHandler() {}

DefaultHandler::~DefaultHandler() noexcept { destroy(); }

RC DefaultHandler::init(const char *base_dir, const char *trx_kit_name, const char *log_handler_name)
{
  // 检查目录是否存在，或者创建
  std::filesystem::path db_dir(base_dir);
  db_dir /= "db";
  error_code ec;
  if (!std::filesystem::is_directory(db_dir) && !std::filesystem::create_directories(db_dir, ec)) {
    LOG_ERROR("Cannot access base dir: %s. msg=%d:%s", db_dir.c_str(), errno, strerror(errno));
    return RC::INTERNAL;
  }

  base_dir_ = base_dir;
  db_dir_   = db_dir;
  trx_kit_name_ = trx_kit_name;
  log_handler_name_ = log_handler_name;

  const char *sys_db = "sys";

  RC ret = create_db(sys_db);
  if (ret != RC::SUCCESS && ret != RC::SCHEMA_DB_EXIST) {
    LOG_ERROR("Failed to create system db");
    return ret;
  }

  ret = open_db(sys_db);
  if (ret != RC::SUCCESS) {
    LOG_ERROR("Failed to open system db. rc=%s", strrc(ret));
    return ret;
  }

  printf("[DEBUG] DefaultHandler::init: open_db completed, setting session\n");
  fflush(stdout);
  const char *msg1 = "[DEBUG] DefaultHandler::init: open_db completed\n";
  ssize_t w1 = write(STDOUT_FILENO, msg1, strlen(msg1));
  (void)w1;

  printf("[DEBUG] DefaultHandler::init: About to call Session::default_session()\n");
  fflush(stdout);
  const char *msg1a = "[DEBUG] DefaultHandler::init: About to call Session::default_session()\n";
  ssize_t w1a = write(STDOUT_FILENO, msg1a, strlen(msg1a));
  (void)w1a;

  Session &default_session = Session::default_session();
  
  printf("[DEBUG] DefaultHandler::init: Session::default_session() returned\n");
  fflush(stdout);
  const char *msg1b = "[DEBUG] DefaultHandler::init: Session::default_session() returned\n";
  ssize_t w1b = write(STDOUT_FILENO, msg1b, strlen(msg1b));
  (void)w1b;

  printf("[DEBUG] DefaultHandler::init: About to call set_current_db()\n");
  fflush(stdout);
  const char *msg1c = "[DEBUG] DefaultHandler::init: About to call set_current_db()\n";
  ssize_t w1c = write(STDOUT_FILENO, msg1c, strlen(msg1c));
  (void)w1c;

  default_session.set_current_db(sys_db);

  printf("[DEBUG] DefaultHandler::init: set_current_db() returned\n");
  fflush(stdout);
  const char *msg2 = "[DEBUG] DefaultHandler::init: set_current_db() returned\n";
  ssize_t w2 = write(STDOUT_FILENO, msg2, strlen(msg2));
  (void)w2;

  // 暂时注释掉 LOG_INFO，看看是否是日志系统导致阻塞
  // LOG_INFO("Default handler init with %s success", base_dir);
  printf("[INFO] Default handler init with %s success\n", base_dir);
  fflush(stdout);
  
  printf("[DEBUG] DefaultHandler::init: Success message logged, returning SUCCESS\n");
  fflush(stdout);
  const char *msg3 = "[DEBUG] DefaultHandler::init: returning SUCCESS\n";
  ssize_t w3 = write(STDOUT_FILENO, msg3, strlen(msg3));
  (void)w3;
  
  return RC::SUCCESS;
}

void DefaultHandler::destroy()
{
  sync();

  for (const auto &iter : opened_dbs_) {
    delete iter.second;
  }
  opened_dbs_.clear();
}

RC DefaultHandler::create_db(const char *dbname)
{
  if (nullptr == dbname || common::is_blank(dbname)) {
    LOG_WARN("Invalid db name");
    return RC::INVALID_ARGUMENT;
  }

  // 如果对应名录已经存在，返回错误
  std::filesystem::path dbpath = db_dir_ / dbname;
  if (std::filesystem::is_directory(dbpath)) {
    LOG_WARN("Db already exists: %s", dbname);
    return RC::SCHEMA_DB_EXIST;
  }

  error_code ec;
  if (!std::filesystem::create_directories(dbpath, ec)) {
    LOG_ERROR("Create db fail: %s. error=%s", dbpath.c_str(), strerror(errno));
    return RC::IOERR_WRITE;
  }
  return RC::SUCCESS;
}

RC DefaultHandler::drop_db(const char *dbname) { return RC::INTERNAL; }

RC DefaultHandler::open_db(const char *dbname)
{
  if (nullptr == dbname || common::is_blank(dbname)) {
    LOG_WARN("Invalid db name");
    return RC::INVALID_ARGUMENT;
  }

  if (opened_dbs_.find(dbname) != opened_dbs_.end()) {
    return RC::SUCCESS;
  }

  std::filesystem::path dbpath = db_dir_ / dbname;
  if (!std::filesystem::is_directory(dbpath)) {
    return RC::SCHEMA_DB_NOT_EXIST;
  }

  // open db
  Db *db  = new Db();
  RC  ret = RC::SUCCESS;
  if ((ret = db->init(dbname, dbpath.c_str(), trx_kit_name_.c_str(), log_handler_name_.c_str())) != RC::SUCCESS) {
    LOG_ERROR("Failed to open db: %s. error=%s", dbname, strrc(ret));
    delete db;
  } else {
    opened_dbs_[dbname] = db;
  }
  return ret;
}

RC DefaultHandler::close_db(const char *dbname) { return RC::UNIMPLEMENTED; }

RC DefaultHandler::create_table(const char *dbname, const char *relation_name, span<const AttrInfoSqlNode> attributes)
{
  Db *db = find_db(dbname);
  if (db == nullptr) {
    return RC::SCHEMA_DB_NOT_OPENED;
  }
  return db->create_table(relation_name, attributes);
}

RC DefaultHandler::drop_table(const char *dbname, const char *relation_name) { return RC::UNIMPLEMENTED; }

Db *DefaultHandler::find_db(const char *dbname) const
{
  std::map<string, Db *>::const_iterator iter = opened_dbs_.find(dbname);
  if (iter == opened_dbs_.end()) {
    return nullptr;
  }
  return iter->second;
}

Table *DefaultHandler::find_table(const char *dbname, const char *table_name) const
{
  if (dbname == nullptr || table_name == nullptr) {
    LOG_WARN("Invalid argument. dbname=%p, table_name=%p", dbname, table_name);
    return nullptr;
  }
  Db *db = find_db(dbname);
  if (nullptr == db) {
    return nullptr;
  }

  return db->find_table(table_name);
}

RC DefaultHandler::sync()
{
  RC rc = RC::SUCCESS;
  for (const auto &db_pair : opened_dbs_) {
    Db *db = db_pair.second;
    rc     = db->sync();
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to sync db. name=%s, rc=%d:%s", db->name(), rc, strrc(rc));
      return rc;
    }
  }
  return rc;
}
