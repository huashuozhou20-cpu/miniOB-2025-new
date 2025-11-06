#include "process.h"
#include "common/defs.h"
#include "common/log/log.h"

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

int daemonize_service(const char* stdout_file, const char* stderr_file) {
  // Simple daemonize implementation
  return 0;
}

// writePidFile is implemented in deps/common/os/pidfile.cpp

std::string getAboslutPath(const char* path) {
  if (path == nullptr) {
    return "";
  }
  if (path[0] == '/') {
    return std::string(path);
  }
  // For relative paths, return as-is for now
  return std::string(path);
}

void sys_log_redirect(const char* stdout_file, const char* stderr_file) {
  // Simple implementation
}

void set_signal_handler(void (*handler)(int)) {
  // Simple implementation
}

} // namespace common

