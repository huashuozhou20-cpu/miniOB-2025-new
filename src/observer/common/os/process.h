#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <sstream>
#include <cstdint>
#include "common/log/log.h"

namespace common {

class ProcessParam {
public:
    std::string get_process_name() const { return process_name_; }
    bool is_demon() const { return is_demon_; }
    std::string get_std_out() const { return std_out_; }
    std::string get_std_err() const { return std_err_; }
    std::string trx_kit_name() const { return trx_kit_name_; }
    std::string durability_mode() const { return durability_mode_; }
    std::string get_conf() const { return conf_; }
    int get_server_port() const { return server_port_; }
    std::string get_protocol() const { return protocol_; }
    std::string get_unix_socket_path() const { return unix_socket_path_; }
    std::string thread_handling_name() const { return thread_handling_name_; }
    
    void init_default(const std::string& name) { process_name_ = name; }
    void set_unix_socket_path(const std::string& path) { unix_socket_path_ = path; }
    void set_server_port(int port) { server_port_ = port; }
    void set_protocol(const std::string& protocol) { protocol_ = protocol; }
    void set_conf(const std::string& conf) { conf_ = conf; }
    void set_std_out(const std::string& out) { std_out_ = out; }
    void set_std_err(const std::string& err) { std_err_ = err; }
    void set_trx_kit_name(const std::string& name) { trx_kit_name_ = name; }
    void set_thread_handling_name(const std::string& name) { thread_handling_name_ = name; }
    void set_buffer_pool_memory_size(int size) { buffer_pool_memory_size_ = size; }
    void set_durability_mode(const std::string& mode) { durability_mode_ = mode; }
    
private:
    std::string process_name_;
    bool is_demon_ = false;
    std::string std_out_;
    std::string std_err_;
    std::string trx_kit_name_;
    std::string durability_mode_;
    std::string conf_;
    int server_port_ = 0;
    std::string protocol_;
    std::string unix_socket_path_;
    std::string thread_handling_name_;
    int buffer_pool_memory_size_ = 0;
};

class Ini {
public:
    std::map<std::string, std::string> get(const std::string& section) const {
        auto it = sections_.find(section);
        if (it != sections_.end()) {
            return it->second;
        }
        return std::map<std::string, std::string>();
    }
    
    int load(const std::string& filename) { return 0; }
    void to_string(std::string& output) const { output = ""; }
    
private:
    std::map<std::string, std::map<std::string, std::string>> sections_;
};

ProcessParam* the_process_param();
Ini* get_properties();

// Helper functions
std::string get_process_name(const char* argv0);
int daemonize_service(const char* stdout_file, const char* stderr_file);
// writePidFile is declared in common/os/pidfile.h
std::string getAboslutPath(const char* path);
void sys_log_redirect(const char* stdout_file, const char* stderr_file);
void set_signal_handler(void (*handler)(int));

}
