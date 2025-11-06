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

namespace common {

/**
 * @brief Runnable interface similar to Java Runnable
 */
class Runnable
{
public:
  virtual ~Runnable() = default;
  virtual void run() = 0;
};

/**
 * @brief Function wrapper for Runnable
 */
class FunctionRunnable : public Runnable
{
public:
  explicit FunctionRunnable(std::function<void()> func) : func_(func) {}
  
  void run() override {
    if (func_) {
      func_();
    }
  }

private:
  std::function<void()> func_;
};

}  // namespace common


