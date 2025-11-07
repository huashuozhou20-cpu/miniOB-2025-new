#include "process.h"
#include "common/defs.h"
#include "common/log/log.h"
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

namespace common {

// Constants
// STATUS_SUCCESS is already defined in common/defs.h as enum value
// SYS_OUTPUT_FILE_POS and SYS_OUTPUT_ERROR are macros in common/log/log.h

ProcessParam* the_process_param()
{
  static ProcessParam* param = nullptr;
  if (param == nullptr) {
    param = new ProcessParam();
  }
  return param;
}

Ini* get_properties()
{
  static Ini* properties = nullptr;
  if (properties == nullptr) {
    properties = new Ini();
  }
  return properties;
}

std::string get_process_name(const char* argv0)
{
  if (argv0 == nullptr) {
    return "miniob";
  }
  std::string path(argv0);
  size_t pos = path.find_last_of('/');
  if (pos != std::string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

int daemonize_service(const char* stdout_file, const char* stderr_file)
{
  // 简化版：仅重定向输出，不做 double-fork，保持与当前测试框架兼容
  sys_log_redirect(stdout_file, stderr_file);
  return 0;
}

// writePidFile is implemented in deps/common/os/pidfile.cpp

std::string getAboslutPath(const char* path)
{
  if (path == nullptr || *path == '\0') {
    return "";
  }
  if (path[0] == '/') {
    return std::string(path);
  }
  char buf[PATH_MAX] = {0};
  if (realpath(path, buf) != nullptr) {
    return std::string(buf);
  }
  // realpath 失败时，自己拼接 CWD
  char cwd[PATH_MAX] = {0};
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    std::string abs = std::string(cwd);
    if (!abs.empty() && abs.back() != '/') abs.push_back('/');
    abs.append(path);
    return abs;
  }
  return std::string(path);
}

void sys_log_redirect(const char* stdout_file, const char* stderr_file)
{
  if (stdout_file && *stdout_file) {
    FILE *fout = freopen(stdout_file, "a", stdout);
    if (fout == nullptr) {
      LOG_WARN("redirect stdout failed. file=%s", stdout_file);
    }
  }
  if (stderr_file && *stderr_file) {
    FILE *ferr = freopen(stderr_file, "a", stderr);
    if (ferr == nullptr) {
      LOG_WARN("redirect stderr failed. file=%s", stderr_file);
    }
  }
}

void set_signal_handler(void (*handler)(int))
{
  // 默认忽略 SIGPIPE，避免写断开连接时进程被杀
  signal(SIGPIPE, SIG_IGN);
  if (handler != nullptr) {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
  }
}

} // namespace common

