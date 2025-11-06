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

#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace common {

/**
 * @brief Simple thread pool executor similar to Java ThreadPoolExecutor
 */
class ThreadPoolExecutor
{
public:
  ThreadPoolExecutor() : shutdown_(false) {}
  
  ~ThreadPoolExecutor() {
    shutdown();
    await_termination();
  }

  /**
   * @brief Initialize thread pool
   * @param name Thread pool name
   * @param core_size Core thread pool size
   * @param max_size Maximum thread pool size
   * @param keep_alive_time Keep alive time in milliseconds
   * @return 0 on success, non-zero on error
   */
  int init(const char *name, int core_size, int max_size, int keep_alive_time) {
    name_ = name;
    core_size_ = core_size;
    max_size_ = max_size;
    keep_alive_time_ = keep_alive_time;
    shutdown_ = false;
    
    // Start core threads
    for (int i = 0; i < core_size; ++i) {
      threads_.emplace_back(&ThreadPoolExecutor::worker_thread, this);
    }
    
    return 0;
  }

  /**
   * @brief Execute a task
   * @param task Task to execute
   * @return 0 on success, non-zero on error
   */
  int execute(std::function<void()> task) {
    if (shutdown_) {
      return -1;
    }
    
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      tasks_.push(task);
    }
    
    condition_.notify_one();
    
    // Create additional threads if needed (up to max_size)
    {
      std::lock_guard<std::mutex> lock(threads_mutex_);
      if (threads_.size() < static_cast<size_t>(max_size_) && 
          tasks_.size() > threads_.size()) {
        threads_.emplace_back(&ThreadPoolExecutor::worker_thread, this);
      }
    }
    
    return 0;
  }

  /**
   * @brief Shutdown thread pool (stop accepting new tasks)
   */
  void shutdown() {
    shutdown_ = true;
    condition_.notify_all();
  }

  /**
   * @brief Wait for all tasks to complete
   */
  void await_termination() {
    {
      std::lock_guard<std::mutex> lock(threads_mutex_);
      for (auto &thread : threads_) {
        if (thread.joinable()) {
          thread.join();
        }
      }
      threads_.clear();
    }
  }

private:
  void worker_thread() {
    while (!shutdown_ || !tasks_.empty()) {
      std::function<void()> task;
      
      {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        condition_.wait(lock, [this] {
          return !tasks_.empty() || shutdown_;
        });
        
        if (tasks_.empty() && shutdown_) {
          break;
        }
        
        if (!tasks_.empty()) {
          task = tasks_.front();
          tasks_.pop();
        }
      }
      
      if (task) {
        task();
      }
    }
  }

private:
  std::string name_;
  int core_size_;
  int max_size_;
  int keep_alive_time_;
  std::atomic<bool> shutdown_;
  
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  
  std::vector<std::thread> threads_;
  std::mutex threads_mutex_;
};

}  // namespace common


