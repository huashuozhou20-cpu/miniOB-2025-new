/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once
#include <mutex>
#include <shared_mutex>
#include <tuple>

namespace common {

// Mutex wrapper for compatibility
class Mutex {
public:
    void lock() { mutex_.lock(); }
    void unlock() { mutex_.unlock(); }
    bool try_lock() { return mutex_.try_lock(); }
    
private:
    std::mutex mutex_;
};

// Shared mutex wrapper
class SharedMutex {
public:
    void lock() { mutex_.lock(); }
    void unlock() { mutex_.unlock(); }
    bool try_lock() { return mutex_.try_lock(); }
    void lock_shared() { mutex_.lock_shared(); }
    void unlock_shared() { mutex_.unlock_shared(); }
    bool try_lock_shared() { return mutex_.try_lock_shared(); }
    
private:
    std::shared_mutex mutex_;
};

// Recursive shared mutex wrapper
class RecursiveSharedMutex {
public:
    void read_latch() { mutex_.lock_shared(); }
    void read_unlatch() { mutex_.unlock_shared(); }
    void write_latch() { mutex_.lock(); }
    void write_unlatch() { mutex_.unlock(); }
    bool try_read_latch() { return mutex_.try_lock_shared(); }
    void lock() { mutex_.lock(); }  // Alias for write_latch
    void unlock() { mutex_.unlock(); }  // Alias for write_unlatch
    void lock_shared() { mutex_.lock_shared(); }
    void unlock_shared() { mutex_.unlock_shared(); }
    bool try_lock_shared() { return mutex_.try_lock_shared(); }
    
private:
    std::shared_mutex mutex_;
};

// Debug mutex - placeholder for debugging
class DebugMutex {
public:
    void lock() {}
    void unlock() {}
    bool try_lock() { return true; }
};

// Lock guard for mutex
template<typename MutexType>
class lock_guard {
public:
    explicit lock_guard(MutexType& m) : mutex_(m) {
        mutex_.lock();
    }
    ~lock_guard() {
        mutex_.unlock();
    }
    
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
    
private:
    MutexType& mutex_;
};

// scoped_lock for single mutex (simplified version)
template<typename MutexType>
class scoped_lock {
public:
    explicit scoped_lock(MutexType& m) : mutex_(m) {
        mutex_.lock();
    }
    ~scoped_lock() {
        mutex_.unlock();
    }
    
    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;
    
private:
    MutexType& mutex_;
};

// Type alias for mutex
using mutex = std::mutex;

} // namespace common

// Helper function for lbt (line backtrace) - placeholder
inline const char* lbt() {
    return "";  // Return empty string for now
}
