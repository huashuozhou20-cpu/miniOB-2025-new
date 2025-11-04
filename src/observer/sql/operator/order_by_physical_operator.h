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
// Created on 2024/10/15.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"
#include <vector>

/**
 * @brief 更新物理算子
 * @ingroup PhysicalOperator
 */
class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator(std::vector<std::unique_ptr<Expression>>&& order_by, std::vector<bool>&& is_asc, int limit);

  virtual ~OrderByPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  RC next(Tuple *upper_tuple) override;

  Tuple *current_tuple() override;
  int    limit() { return limit_; }

private:
  RC fetch_next();
  RC     quick_sort(Tuple *upper_tuple = nullptr);
  RC     external_sort(Tuple *upper_tuple = nullptr);
  RC     limit_sort(Tuple *upper_tuple = nullptr);
  bool     cmp(const vector<Value>& a_vals, const vector<Value>& b_vals);
  
  // External sort helpers
  RC write_chunk_to_file(const vector<ValueListTuple> &chunk, const vector<vector<Value>> &chunk_values, 
                         const vector<size_t> &chunk_ids, const string &filename);
  RC read_chunk_from_file(const string &filename, vector<ValueListTuple> &chunk, 
                          vector<vector<Value>> &chunk_values, vector<size_t> &chunk_ids);
  RC merge_sorted_chunks(const vector<string> &chunk_files, const string &output_file);
  void cleanup_temp_files(const vector<string> &files);

private:
  static constexpr size_t MEMORY_THRESHOLD = 10000;  // Threshold for external sort
  static constexpr size_t CHUNK_SIZE = 5000;         // Size of each chunk for external sort
  
  std::vector<std::unique_ptr<Expression>> order_by_;
  std::vector<bool>                        is_asc_;
  bool                                     first_emited_ = false;  /// 第一条数据是否已经输出
  bool                                     have_value = false;
  vector<ValueListTuple>                   value_list_;
  vector<size_t>                           ids_;     
  vector<vector<Value>>                    order_values_;        
  size_t                                   current_id_;
  int                                      limit_ = -1;
  
  // External sort state
  string                                   external_sort_file_;  // Final merged file
  vector<string>                           temp_files_;          // Temporary files to cleanup
};
