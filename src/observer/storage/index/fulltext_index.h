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

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "storage/index/index.h"
#include "storage/record/record.h"
class Table;
class FieldMeta;
class IndexMeta;

/**
 * @brief 全文索引
 * @ingroup Index
 * @details 使用倒排索引实现全文搜索
 */
class FullTextIndex : public Index
{
public:
  FullTextIndex() = default;
  virtual ~FullTextIndex() noexcept = default;

  RC create(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta);
  RC open(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta);
  RC close();
  RC drop() override;

  bool is_fulltext_index() { return true; }

  RC insert_entry(const char *record, const RID *rid) override;
  RC update_entry(const char *record, const RID *rid) override;
  RC delete_entry(const char *record, const RID *rid) override;

  /**
   * @brief 全文搜索
   * @param query 搜索查询字符串
   * @return 匹配的 RID 列表
   */
  std::vector<RID> search(const std::string &query);

  /**
   * @brief 计算 BM25 评分
   * @param query 查询字符串
   * @param rid 文档 RID
   * @return BM25 分数
   */
  double calculate_bm25_score(const std::string &query, const RID &rid);

  IndexScanner *create_scanner(const char *left_key, int left_len, bool left_inclusive,
                                             const char *right_key, int right_len, bool right_inclusive) override;

  RC sync() override;

private:
  /**
   * @brief 分词
   * @param text 输入文本
   * @return 分词后的词列表
   */
  std::vector<std::string> tokenize(const std::string &text);

  /**
   * @brief 标准化词（转小写，去除标点）
   * @param word 输入词
   * @return 标准化后的词
   */
  std::string normalize_word(const std::string &word);

private:
  bool                        inited_ = false;
  Table                      *table_  = nullptr;
  const FieldMeta            *field_meta_ = nullptr;
  
  // 倒排索引: word -> set of RIDs
  std::unordered_map<std::string, std::unordered_set<RID,RIDHash>> inverted_index_;
  
  // 文档索引: RID -> set of words (用于删除和更新)
  std::unordered_map<RID, std::unordered_set<std::string>,RIDHash> document_index_;
  
  // BM25 参数
  static constexpr double BM25_K1 = 1.5;
  static constexpr double BM25_B = 0.75;
  
  // 文档长度（用于 BM25 计算）
  std::unordered_map<RID, size_t,RIDHash> document_lengths_;
  
  // 平均文档长度（用于 BM25 计算）
  double avg_document_length_ = 0.0;
};

