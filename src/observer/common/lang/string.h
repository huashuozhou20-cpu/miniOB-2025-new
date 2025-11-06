#pragma once
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

namespace common {
using string = std::string;

inline bool is_blank(const char* str) {
    if (str == nullptr) return true;
    while (*str != '\0') {
        if (*str != ' ' && *str != '\t' && *str != '\n' && *str != '\r') {
            return false;
        }
        str++;
    }
    return true;
}

inline int str_compare(const char* s1, const char* s2) {
    if (s1 == nullptr && s2 == nullptr) return 0;
    if (s1 == nullptr) return -1;
    if (s2 == nullptr) return 1;
    return strcmp(s1, s2);
}

// Strip whitespace from string (in-place)
inline void strip(std::string& str) {
    if (str.empty()) return;
    
    // Remove leading whitespace
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        str.erase(0, start);
    } else {
        str.clear();
        return;
    }
    
    // Remove trailing whitespace
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) {
        str.erase(end + 1);
    }
}

// Split string by delimiter
inline void split_string(const std::string& str, const std::string& delim, std::vector<std::string>& result) {
    result.clear();
    if (str.empty()) return;
    
    size_t start = 0;
    size_t pos = 0;
    
    while ((pos = str.find(delim, start)) != std::string::npos) {
        if (pos > start) {
            result.push_back(str.substr(start, pos - start));
        }
        start = pos + delim.length();
    }
    
    // Add the last part
    if (start < str.length()) {
        result.push_back(str.substr(start));
    } else if (start == str.length() && !result.empty()) {
        // Handle trailing delimiter
        result.push_back("");
    }
}

// File path separator string - defined in deps/common/defs.h
// Use the one from deps/common/defs.h to avoid conflict

// Substring function for C-style strings (used by bison-generated code)
inline char* substr(const char* str, size_t pos, size_t len) {
    if (str == nullptr) return nullptr;
    size_t str_len = strlen(str);
    if (pos >= str_len) return nullptr;
    size_t copy_len = (len > str_len - pos) ? (str_len - pos) : len;
    char* result = (char*)malloc(copy_len + 1);
    if (result == nullptr) return nullptr;
    memcpy(result, str + pos, copy_len);
    result[copy_len] = '\0';
    return result;
}
}

