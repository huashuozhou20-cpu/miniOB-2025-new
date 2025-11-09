/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "storage/index/fulltext_index.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/table/table.h"
#include "storage/record/record_manager.h"
#include "storage/field/field_meta.h"
#include "common/value.h"
#include "storage/index/index_meta.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

RC FullTextIndex::create(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta)
{
  if (table == nullptr || file_name == nullptr) {
    LOG_WARN("Invalid arguments. table=%p, file_name=%p", table, file_name);
    return RC::INVALID_ARGUMENT;
  }

  if (field_meta.type() != AttrType::CHARS && field_meta.type() != AttrType::TEXTS) {
    LOG_WARN("Fulltext index can only be created on CHAR or TEXT fields");
    return RC::INVALID_ARGUMENT;
  }

  table_ = table;
  field_meta_ = &field_meta;
  
  // 初始化索引元数据
  RC rc = init(index_meta, {&field_meta});
  if (OB_FAIL(rc)) {
    LOG_WARN("Failed to init index meta. rc=%s", strrc(rc));
    return rc;
  }

  // 创建索引文件（如果不存在）
  ofstream fs(file_name, ios::binary | ios::out);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to create fulltext index file: %s", file_name);
    return RC::IOERR_OPEN;
  }
  fs.close();

  inited_ = true;
  LOG_INFO("Successfully created fulltext index. file=%s", file_name);
  return RC::SUCCESS;
}

RC FullTextIndex::open(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta)
{
  if (table == nullptr || file_name == nullptr) {
    LOG_WARN("Invalid arguments. table=%p, file_name=%p", table, file_name);
    return RC::INVALID_ARGUMENT;
  }

  table_ = table;
  field_meta_ = &field_meta;
  
  RC rc = init(index_meta, {&field_meta});
  if (OB_FAIL(rc)) {
    LOG_WARN("Failed to init index meta. rc=%s", strrc(rc));
    return rc;
  }

  // TODO: 从文件加载索引数据
  // 目前先简单实现，不持久化索引数据
  
  inited_ = true;
  LOG_INFO("Successfully opened fulltext index. file=%s", file_name);
  return RC::SUCCESS;
}

RC FullTextIndex::close()
{
  if (!inited_) {
    return RC::SUCCESS;
  }

  // TODO: 持久化索引数据到文件
  
  inverted_index_.clear();
  document_index_.clear();
  inited_ = false;
  
  return RC::SUCCESS;
}

RC FullTextIndex::drop()
{
  if (!inited_) {
    return RC::SUCCESS;
  }

  inverted_index_.clear();
  document_index_.clear();
  inited_ = false;
  
  return RC::SUCCESS;
}

RC FullTextIndex::insert_entry(const char *record, const RID *rid)
{
  if (record == nullptr || rid == nullptr || !inited_) {
    LOG_WARN("Invalid arguments or index not initialized");
    return RC::INVALID_ARGUMENT;
  }

  // 获取字段值
  Value value;
  const char *field_data = record + field_meta_->offset();
  if (field_meta_->type() == AttrType::CHARS) {
    value = Value(field_data, field_meta_->len());
  } else if (field_meta_->type() == AttrType::TEXTS) {
    // 从 text buffer 读取
    int64_t offset = 0, length = 0;
    memcpy(&offset, field_data, sizeof(int64_t));
    memcpy(&length, field_data + sizeof(int64_t), sizeof(int64_t));
    
    if (offset >= 0 && length > 0) {
      char *text_data = new char[length + 1];
      RC rc = table_->read_text(offset, length, text_data);
      if (rc == RC::SUCCESS) {
        text_data[length] = '\0';
        value = Value(text_data, length);
      }
      delete[] text_data;
      if (rc != RC::SUCCESS) {
        return rc;
      }
    } else {
      return RC::SUCCESS;  // NULL or empty text
    }
  } else {
    return RC::INVALID_ARGUMENT;
  }

  if (value.attr_type() == AttrType::NULLS) {
    return RC::SUCCESS;  // NULL值不索引
  }

  // 获取文本内容
  string text = value.to_string();
  if (text.empty()) {
    return RC::SUCCESS;
  }

  // 分词
  vector<string> words = tokenize(text);
  
  // 删除旧索引（如果存在）
  if (document_index_.find(*rid) != document_index_.end()) {
    delete_entry(record, rid);
  }

  // 添加到倒排索引
  unordered_set<string> doc_words;
  size_t doc_length = 0;
  for (const string &word : words) {
    string normalized = normalize_word(word);
    if (!normalized.empty()) {
      inverted_index_[normalized].insert(*rid);
      doc_words.insert(normalized);
      doc_length++;
    }
  }

  // 更新文档索引
  document_index_[*rid] = doc_words;
  document_lengths_[*rid] = doc_length;
  
  // 更新平均文档长度
  if (document_index_.size() > 0) {
    size_t total_length = 0;
    for (const auto &pair : document_lengths_) {
      total_length += pair.second;
    }
    avg_document_length_ = static_cast<double>(total_length) / document_index_.size();
  }

  return RC::SUCCESS;
}

RC FullTextIndex::update_entry(const char *record, const RID *rid)
{
  // 更新 = 删除旧 + 插入新
  delete_entry(record, rid);
  return insert_entry(record, rid);
}

RC FullTextIndex::delete_entry(const char *record, const RID *rid)
{
  if (rid == nullptr || !inited_) {
    return RC::INVALID_ARGUMENT;
  }

  // 从文档索引中获取该文档的所有词
  auto it = document_index_.find(*rid);
  if (it == document_index_.end()) {
    return RC::SUCCESS;  // 文档不存在，无需删除
  }

  // 从倒排索引中删除该文档的所有词
  for (const string &word : it->second) {
    auto word_it = inverted_index_.find(word);
    if (word_it != inverted_index_.end()) {
      word_it->second.erase(*rid);
      // 如果该词没有文档了，删除该词
      if (word_it->second.empty()) {
        inverted_index_.erase(word_it);
      }
    }
  }

  // 从文档索引中删除
  document_index_.erase(it);
  
  // 从文档长度索引中删除
  document_lengths_.erase(*rid);
  
  // 更新平均文档长度
  if (document_index_.size() > 0) {
    size_t total_length = 0;
    for (const auto &pair : document_lengths_) {
      total_length += pair.second;
    }
    avg_document_length_ = static_cast<double>(total_length) / document_index_.size();
  } else {
    avg_document_length_ = 0.0;
  }

  return RC::SUCCESS;
}

std::vector<RID> FullTextIndex::search(const std::string &query)
{
  if (!inited_ || query.empty()) {
    return {};
  }

  // 对查询字符串分词
  vector<string> query_words = tokenize(query);
  if (query_words.empty()) {
    return {};
  }

  // 标准化查询词
  vector<string> normalized_words;
  for (const string &word : query_words) {
    string normalized = normalize_word(word);
    if (!normalized.empty()) {
      normalized_words.push_back(normalized);
    }
  }

  if (normalized_words.empty()) {
    return {};
  }

  // 计算每个文档的 BM25 分数
  unordered_map<RID, double, RIDHash> doc_scores;
  
  // 计算每个词的 IDF（逆文档频率）
  size_t total_docs = document_index_.size();
  if (total_docs == 0) {
    return {};
  }
  
  unordered_map<string, double> word_idf;
  for (const string &word : normalized_words) {
    auto it = inverted_index_.find(word);
    if (it != inverted_index_.end()) {
      size_t doc_freq = it->second.size();
      // IDF = log((N - df + 0.5) / (df + 0.5))
      // N = total documents, df = documents containing term
      double idf = log((total_docs - doc_freq + 0.5) / (doc_freq + 0.5));
      word_idf[word] = idf;
    }
  }
  
  // 计算每个文档的 BM25 分数
  for (const string &word : normalized_words) {
    auto word_it = inverted_index_.find(word);
    if (word_it == inverted_index_.end()) {
      continue;
    }
    
    double idf = word_idf[word];
    
    for (const RID &rid : word_it->second) {
      // 计算词在该文档中的词频
      auto doc_it = document_index_.find(rid);
      if (doc_it == document_index_.end()) {
        continue;
      }
      
      size_t term_freq = 0;
      for (const string &doc_word : doc_it->second) {
        if (doc_word == word) {
          term_freq++;
        }
      }
      
      // 获取文档长度
      size_t doc_len = document_lengths_.find(rid)->second;
      
      // BM25 score = IDF * (f * (k1 + 1)) / (f + k1 * (1 - b + b * |d| / avgdl))
      // f = term frequency in document
      // k1 = 1.5, b = 0.75
      double doc_len_norm = doc_len / avg_document_length_;
      double numerator = term_freq * (BM25_K1 + 1.0);
      double denominator = term_freq + BM25_K1 * (1.0 - BM25_B + BM25_B * doc_len_norm);
      double score = idf * (numerator / denominator);
      
      doc_scores[rid] += score;
    }
  }

  // 按分数排序，返回匹配的 RID
  vector<pair<RID, double>> sorted_results;
  for (const auto &pair : doc_scores) {
    sorted_results.push_back(pair);
  }

  sort(sorted_results.begin(), sorted_results.end(),
       [](const pair<RID, double> &a, const pair<RID, double> &b) {
         return a.second > b.second;  // 降序排列
       });

  vector<RID> results;
  for (const auto &pair : sorted_results) {
    results.push_back(pair.first);
  }

  return results;
}

IndexScanner *FullTextIndex::create_scanner(const char *left_key, int left_len, bool left_inclusive,
                                             const char *right_key, int right_len, bool right_inclusive)
{
  // 全文索引不支持范围扫描
  return nullptr;
}

RC FullTextIndex::sync()
{
  // TODO: 持久化索引数据
  return RC::SUCCESS;
}

std::vector<std::string> FullTextIndex::tokenize(const std::string &text)
{
  vector<string> tokens;
  if (text.empty()) {
    return tokens;
  }

  istringstream iss(text);
  string word;
  
  while (iss >> word) {
    // 去除标点符号
    string cleaned;
    for (char c : word) {
      if (isalnum(c) || c == '_') {
        cleaned += c;
      }
    }
    if (!cleaned.empty()) {
      tokens.push_back(cleaned);
    }
  }

  return tokens;
}

std::string FullTextIndex::normalize_word(const std::string &word)
{
  string normalized;
  normalized.reserve(word.size());
  
  for (char c : word) {
    normalized += tolower(c);
  }
  
  return normalized;
}

double FullTextIndex::calculate_bm25_score(const std::string &query, const RID &rid)
{
  if (!inited_ || query.empty()) {
    return 0.0;
  }

  // 对查询字符串分词
  vector<string> query_words = tokenize(query);
  if (query_words.empty()) {
    return 0.0;
  }

  // 标准化查询词
  vector<string> normalized_words;
  for (const string &word : query_words) {
    string normalized = normalize_word(word);
    if (!normalized.empty()) {
      normalized_words.push_back(normalized);
    }
  }

  if (normalized_words.empty()) {
    return 0.0;
  }

  // 检查文档是否存在
  auto doc_it = document_index_.find(rid);
  if (doc_it == document_index_.end()) {
    return 0.0;
  }

  // 计算 BM25 分数
  size_t total_docs = document_index_.size();
  if (total_docs == 0 || avg_document_length_ == 0.0) {
    return 0.0;
  }

  double total_score = 0.0;
  
  for (const string &word : normalized_words) {
    auto word_it = inverted_index_.find(word);
    if (word_it == inverted_index_.end()) {
      continue;
    }
    
    // 计算 IDF
    size_t doc_freq = word_it->second.size();
    double idf = log((total_docs - doc_freq + 0.5) / (doc_freq + 0.5));
    
    // 计算词频
    size_t term_freq = 0;
    for (const string &doc_word : doc_it->second) {
      if (doc_word == word) {
        term_freq++;
      }
    }
    
    if (term_freq == 0) {
      continue;
    }
    
    // 获取文档长度
    auto len_it = document_lengths_.find(rid);
    if (len_it == document_lengths_.end()) {
      continue;
    }
    size_t doc_len = len_it->second;
    
    // BM25 score = IDF * (f * (k1 + 1)) / (f + k1 * (1 - b + b * |d| / avgdl))
    double doc_len_norm = doc_len / avg_document_length_;
    double numerator = term_freq * (BM25_K1 + 1.0);
    double denominator = term_freq + BM25_K1 * (1.0 - BM25_B + BM25_B * doc_len_norm);
    double score = idf * (numerator / denominator);
    
    total_score += score;
  }

  return total_score;
}

