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
// Created by WangYunlai on 2022/12/30.
//

#include "sql/operator/join_physical_operator.h"
#include "common/log/log.h"

NestedLoopJoinPhysicalOperator::NestedLoopJoinPhysicalOperator() {}

RC NestedLoopJoinPhysicalOperator::open(Trx *trx)
{
  LOG_INFO("open join operator");
  if (children_.size() != 2) {
    LOG_WARN("nlj operator should have 2 children");
    return RC::INTERNAL;
  }

  RC rc         = RC::SUCCESS;
  left_         = children_[0].get();
  right_        = children_[1].get();
  right_closed_ = true;
  round_done_   = true;

  rc   = left_->open(trx);
  trx_ = trx;
  return rc;
}

RC NestedLoopJoinPhysicalOperator::next()
{
  bool left_need_step = (left_tuple_ == nullptr);
  RC   rc             = RC::SUCCESS;
  if (round_done_) {
    left_need_step = true;
  } else {
    rc = right_next();
    if (rc != RC::SUCCESS) {
      if (rc == RC::RECORD_EOF) {
        left_need_step = true;
      } else {
        return rc;
      }
    } else {
      return rc;  // got one tuple from right
    }
  }

  if (left_need_step) {
    rc = left_next();
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  rc = right_next();
  return rc;
}

RC NestedLoopJoinPhysicalOperator::close()
{
  RC rc = left_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to close left oper. rc=%s", strrc(rc));
  }

  if (!right_closed_) {
    rc = right_->close();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to close right oper. rc=%s", strrc(rc));
    } else {
      right_closed_ = true;
    }
  }

  LOG_INFO("close join operator");
  return rc;
}

Tuple *NestedLoopJoinPhysicalOperator::current_tuple() { return &joined_tuple_; }

RC NestedLoopJoinPhysicalOperator::left_next()
{
  RC rc = RC::SUCCESS;
  rc    = left_->next();
  if (rc != RC::SUCCESS) {
    return rc;
  }

  left_tuple_ = left_->current_tuple();
  joined_tuple_.set_left(left_tuple_);
  return rc;
}

RC NestedLoopJoinPhysicalOperator::right_next()
{
  RC rc = RC::SUCCESS;
  if (round_done_) {
    if (!right_closed_) {
      rc = right_->close();

      right_closed_ = true;
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }

    rc = right_->open(trx_);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    right_closed_ = false;

    round_done_ = false;
  }

  // 循环获取右表的 tuple，直到满足 JOIN 条件或右表遍历完毕
  while (true) {
    rc = right_->next();
    if (rc != RC::SUCCESS) {
      if (rc == RC::RECORD_EOF) {
        round_done_ = true;
      }
      return rc;
    }

    right_tuple_ = right_->current_tuple();
    joined_tuple_.set_right(right_tuple_);

    // 如果有 JOIN 条件，检查是否满足
    if (join_condition_ != nullptr) {
      bool filter_result = false;
      rc = filter_join_condition(filter_result);
      if (rc != RC::SUCCESS) {
        // 评估 JOIN 条件时出错
        return rc;
      }
      if (filter_result) {
        // JOIN 条件满足，返回成功
        return RC::SUCCESS;
      } else {
        // JOIN 条件不满足，继续获取下一个 tuple
        continue;
      }
    } else {
      // 没有 JOIN 条件，直接返回
      return RC::SUCCESS;
    }
  }
}

RC NestedLoopJoinPhysicalOperator::filter_join_condition(bool &result)
{
  result = true;
  if (join_condition_ == nullptr) {
    return RC::SUCCESS;
  }

  Value value;
  RC rc = join_condition_->get_value(joined_tuple_, value);
  if (rc != RC::SUCCESS && rc != RC::NULL_TUPLE) {
    LOG_WARN("failed to evaluate join condition. rc=%s", strrc(rc));
    return rc;
  }
  // Handle RC::NULL_TUPLE: if get_value returns NULL_TUPLE, set value to NULL
  if (rc == RC::NULL_TUPLE) {
    value.set_null();
  }

  // 确保正确处理 boolean 值
  // 如果 value 是 NULL 类型，应该返回 false
  if (value.attr_type() == AttrType::NULLS) {
    result = false;
    return RC::SUCCESS;
  }

  result = value.get_boolean();
  return RC::SUCCESS;
}

void NestedLoopJoinPhysicalOperator::set_join_condition(std::unique_ptr<Expression> join_condition)
{
  join_condition_ = std::move(join_condition);
}
