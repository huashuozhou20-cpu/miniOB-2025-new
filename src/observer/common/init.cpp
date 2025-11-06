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
// Created by Longda on 2021/5/3.
//

#include "common/init.h"

#include "common/ini_setting.h"
#include "common/lang/string.h"
#include "common/lang/iostream.h"
#include "common/log/log.h"
#include "common/os/path.h"
#include "common/os/pidfile.h"
#include "os/process.h"
#include "common/os/signal.h"
#include "global_context.h"
#include "session/session.h"
// #include "session/session_stage.h"
// #include "sql/plan_cache/plan_cache_stage.h"
#include "storage/buffer/disk_buffer_pool.h"
#include "storage/default/default_handler.h"
#include "storage/trx/trx.h"

#include <exception>
#include <unistd.h>  // for write()
#include <cstring>   // for strlen()

using namespace common;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;

bool *&_get_init()
{
  static bool  util_init   = false;
  static bool *util_init_p = &util_init;
  return util_init_p;
}

bool get_init() { return *_get_init(); }

void set_init(bool value) { *_get_init() = value; }

void sig_handler(int sig)
{
  // Signal handler will be add in the next step.
  //  Add action to shutdown

  LOG_INFO("Receive one signal of %d.", sig);
}

int init_log(common::ProcessParam *process_cfg, common::Ini &properties)
{
  const string &proc_name = process_cfg->get_process_name();
  try {
    // we had better alloc one lock to do so, but simplify the logic
    if (g_log) {
      return 0;
    }

    auto log_context_getter = []() -> intptr_t { 
      return reinterpret_cast<intptr_t>(Session::current_session()); 
    };

    const string        log_section_name = "LOG";
    std::map<string, string> log_section      = properties.get(log_section_name);

    string log_file_name;

    // get log file name
    string key = "LOG_FILE_NAME";

    std::map<string, string>::iterator it = log_section.find(key);
    if (it == log_section.end()) {
      log_file_name = proc_name + ".log";
      cout << "Not set log file name, use default " << log_file_name << endl;
    } else {
      log_file_name = it->second;
    }

    log_file_name = getAboslutPath(log_file_name.c_str());

    LOG_LEVEL log_level = LOG_LEVEL_INFO;
    key                 = ("LOG_FILE_LEVEL");
    it                  = log_section.find(key);
    if (it != log_section.end()) {
      int log = (int)log_level;
      str_to_val(it->second, log);
      log_level = (LOG_LEVEL)log;
    }

    LOG_LEVEL console_level = LOG_LEVEL_INFO;
    key                     = ("LOG_CONSOLE_LEVEL");
    it                      = log_section.find(key);
    if (it != log_section.end()) {
      int log = (int)console_level;
      str_to_val(it->second, log);
      console_level = (LOG_LEVEL)log;
    }

    LoggerFactory::init_default(log_file_name, log_level, console_level);
    if (g_log) {
      g_log->set_context_getter(log_context_getter);
    }

    key = ("DefaultLogModules");
    it  = log_section.find(key);
    if (it != log_section.end()) {
      g_log->set_default_module(it->second);
    }

    if (process_cfg->is_demon()) {
      sys_log_redirect(log_file_name.c_str(), log_file_name.c_str());
    }

    return 0;
  } catch (exception &e) {
    cerr << "Failed to init log for " << proc_name << SYS_OUTPUT_FILE_POS << SYS_OUTPUT_ERROR << endl;
    return errno;
  }

  return 0;
}

void cleanup_log()
{

  if (g_log) {
    delete g_log;
    g_log = nullptr;
  }
}

int prepare_init_seda()
{
  return 0;
}

int init_global_objects(common::ProcessParam *process_param, common::Ini &properties)
{
  printf("[DEBUG] init_global_objects: Creating DefaultHandler\n");
  fflush(stdout);
  ssize_t w1 = write(STDOUT_FILENO, "[DEBUG] init_global_objects: Creating DefaultHandler\n", 52);
  (void)w1;
  
  GCTX.handler_ = new DefaultHandler();

  int ret = 0;

  printf("[DEBUG] init_global_objects: Calling handler->init()\n");
  fflush(stdout);
  ssize_t w2 = write(STDOUT_FILENO, "[DEBUG] init_global_objects: Calling handler->init()\n", 52);
  (void)w2;
  
  RC rc = GCTX.handler_->init("miniob", 
                              process_param->trx_kit_name().c_str(),
                              process_param->durability_mode().c_str());
  
  printf("[DEBUG] init_global_objects: handler->init() returned\n");
  fflush(stdout);
  ssize_t w3 = write(STDOUT_FILENO, "[DEBUG] init_global_objects: handler->init() returned\n", 52);
  (void)w3;
  
  if (OB_FAIL(rc)) {
    printf("[ERROR] failed to init handler. rc=%s\n", strrc(rc));
    fflush(stdout);
    LOG_ERROR("failed to init handler. rc=%s", strrc(rc));
    return -1;
  }
  
  printf("[DEBUG] init_global_objects: handler->init() completed successfully\n");
  fflush(stdout);
  ssize_t w4 = write(STDOUT_FILENO, "[DEBUG] init_global_objects: handler->init() completed successfully\n", 65);
  (void)w4;
  
  printf("[DEBUG] init_global_objects: About to return %d\n", ret);
  fflush(stdout);
  ssize_t w5 = write(STDOUT_FILENO, "[DEBUG] init_global_objects: About to return\n", 45);
  (void)w5;
  
  return ret;
}

int uninit_global_objects()
{
  delete GCTX.handler_;
  GCTX.handler_ = nullptr;

  return 0;
}

int init(common::ProcessParam *process_param)
{
  if (get_init()) {
    return 0;
  }

  set_init(true);

  // Run as daemon if daemonization requested
  int rc = STATUS_SUCCESS;
  if (process_param->is_demon()) {
    rc = daemonize_service(process_param->get_std_out().c_str(), process_param->get_std_err().c_str());
    if (rc != 0) {
      cerr << "Shutdown due to failed to daemon current process!" << endl;
      return rc;
    }
  }

  writePidFile(process_param->get_process_name().c_str());

  // Initialize global variables before enter multi-thread mode
  // to avoid race condition

  // Read Configuration files
  rc = common::get_properties()->load(process_param->get_conf());
  if (rc) {
    cerr << "Failed to load configuration files" << endl;
    return rc;
  }

  // Init tracer
  rc = init_log(process_param, *common::get_properties());
  if (rc) {
    cerr << "Failed to init Log" << endl;
    return rc;
  }

  string conf_data;
  common::get_properties()->to_string(conf_data);
  LOG_INFO("Output configuration \n%s", conf_data.c_str());

  printf("[DEBUG] About to call init_global_objects()\n");
  fflush(stdout);
  ssize_t w1 = write(STDOUT_FILENO, "[DEBUG] About to call init_global_objects()\n", 45);
  (void)w1;
  
  rc = init_global_objects(process_param, *common::get_properties());
  
  printf("[DEBUG] init_global_objects() returned with code: %d\n", rc);
  fflush(stdout);
  ssize_t w2 = write(STDOUT_FILENO, "[DEBUG] init_global_objects() returned\n", 40);
  (void)w2;
  
  if (rc != 0) {
    LOG_ERROR("failed to init global objects");
    return rc;
  }

  printf("[DEBUG] After checking init_global_objects return code\n");
  fflush(stdout);
  ssize_t w3 = write(STDOUT_FILENO, "[DEBUG] After checking init_global_objects return code\n", 54);
  (void)w3;

  // Block interrupt signals before creating child threads.
  // setSignalHandler(sig_handler);
  // sigset_t newSigset, oset;
  // blockDefaultSignals(&newSigset, &oset);
  //  wait interrupt signals
  // startWaitForSignals(&newSigset);

  // 先输出到标准输出，避免日志系统阻塞
  printf("[DEBUG] About to log 'Successfully init utility'\n");
  fflush(stdout);
  
  // 暂时注释掉 LOG_INFO，看看是否是日志系统导致阻塞
  // LOG_INFO("Successfully init utility");
  printf("[INFO] Successfully init utility\n");
  fflush(stdout);
  
  printf("[DEBUG] After LOG_INFO, about to return\n");
  fflush(stdout);
  
  cerr << "[DEBUG] init() completed successfully, returning STATUS_SUCCESS" << endl;
  cerr.flush();  // 强制刷新输出
  printf("[DEBUG] init() completed successfully, returning STATUS_SUCCESS\n");
  fflush(stdout);
  
  // 使用系统调用直接输出，绕过所有缓冲
  const char *msg1 = "[DEBUG] Using write() syscall to ensure output\n";
  const char *msg2 = "[DEBUG] Using write() syscall to stderr\n";
  ssize_t ret1 = write(STDOUT_FILENO, msg1, strlen(msg1));
  ssize_t ret2 = write(STDERR_FILENO, msg2, strlen(msg2));
  (void)ret1;  // 忽略返回值
  (void)ret2;  // 忽略返回值

  return STATUS_SUCCESS;
}

void cleanup_util()
{
  uninit_global_objects();

  common::Ini* props = common::get_properties();
  if (nullptr != props) {
    delete props;
    // Note: get_properties() is a function, we can't assign to it
    // In a real implementation, get_properties() would return a reference to a static variable
  }

  LOG_INFO("Shutdown Cleanly!");

  // Finalize tracer
  cleanup_log();

  set_init(false);
}

void cleanup() { cleanup_util(); }
