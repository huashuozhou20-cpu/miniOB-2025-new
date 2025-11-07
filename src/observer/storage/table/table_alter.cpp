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
// Created by Wangyunlai on 2024/12/07.
//

#include "storage/table/table.h"
#include "common/log/log.h"
#include "storage/table/table_meta.h"
#include "storage/db/db.h"
#include "storage/common/meta_util.h"
#include "sql/parser/parse_defs.h"
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

RC Table::alter_table(Trx *trx, int alter_type, const AttrInfoSqlNode &attr_info,
                     const std::string &old_name, const std::string &new_name)
{
  RC rc = RC::SUCCESS;
  
  // 创建新的表元数据
  TableMeta new_table_meta(table_meta_);
  
  switch (static_cast<AlterTableSqlNode::AlterType>(alter_type)) {
    case AlterTableSqlNode::AlterType::ADD_COLUMN: {
      // 添加列
      const int sys_field_num = table_meta_.sys_field_num();
      const int field_num = table_meta_.field_num();
      int new_field_id = field_num - sys_field_num;
      
      // 计算新的字段偏移量
      int field_offset = table_meta_.record_size();
      
      // 创建新字段
      FieldMeta new_field;
      int attr_len = attr_info.type == AttrType::VECTORS ? 
                     attr_info.length * sizeof(float) : attr_info.length;
      rc = new_field.init(attr_info.name.c_str(), attr_info.type, field_offset,
                          attr_len, true /*visible*/, new_field_id, attr_info.nullable);
      if (OB_FAIL(rc)) {
        LOG_ERROR("Failed to init new field. table=%s, field=%s, rc=%s",
                  name(), attr_info.name.c_str(), strrc(rc));
        return rc;
      }
      
      // 更新 null bitmap 字段的长度
      if (new_table_meta.fields_.size() > 0) {
        int old_null_len = (field_num + 7) / 8;
        int new_null_len = (field_num + 1 + 7) / 8;
        if (new_null_len > old_null_len) {
          // 需要更新 null bitmap 字段的长度
          FieldMeta null_field(new_table_meta.fields_[0]);
          null_field = FieldMeta("__null", AttrType::CHARS, 0, new_null_len, false, 0, false);
          new_table_meta.fields_[0] = null_field;
          
          // 调整所有字段的偏移量
          int offset_adjust = new_null_len - old_null_len;
          for (size_t i = 1; i < new_table_meta.fields_.size(); i++) {
            const FieldMeta &old_field = new_table_meta.fields_[i];
            FieldMeta new_field(old_field.name(), old_field.type(), 
                               old_field.offset() + offset_adjust,
                               old_field.len(), old_field.visible(), 
                               old_field.field_id(), old_field.nullable());
            new_table_meta.fields_[i] = new_field;
          }
          field_offset += offset_adjust;
        }
      }
      
      // 添加新字段
      new_field = FieldMeta(attr_info.name.c_str(), attr_info.type, field_offset,
                            attr_len, true, new_field_id, attr_info.nullable);
      new_table_meta.fields_.push_back(new_field);
      new_table_meta.record_size_ += new_field.len();
      
      LOG_INFO("Adding column %s to table %s", attr_info.name.c_str(), name());
      break;
    }
    
    case AlterTableSqlNode::AlterType::DROP_COLUMN: {
      // 删除列
      const FieldMeta *field = new_table_meta.field(old_name.c_str());
      if (field == nullptr) {
        LOG_ERROR("Column does not exist. table=%s, column=%s", name(), old_name.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      
      // 检查是否有索引包含此列
      for (int i = 0; i < static_cast<int>(new_table_meta.index_num()); i++) {
        const IndexMeta *index = new_table_meta.index(i);
        for (const std::string &index_field : index->field()) {
          if (index_field == old_name) {
            LOG_ERROR("Cannot drop column with index. table=%s, column=%s, index=%s",
                      name(), old_name.c_str(), index->name());
            return RC::INVALID_ARGUMENT;
          }
        }
      }
      
      // 从 fields_ 中删除字段
      int field_id = new_table_meta.field_id(old_name.c_str());
      if (field_id < new_table_meta.sys_field_num()) {
        LOG_ERROR("Cannot drop system field. table=%s, column=%s", name(), old_name.c_str());
        return RC::INVALID_ARGUMENT;
      }
      
      // 删除字段
      new_table_meta.fields_.erase(new_table_meta.fields_.begin() + field_id);
      
      // 重新计算 record_size 和字段偏移量
      int field_offset = 0;
      int null_len = (new_table_meta.fields_.size() + 7) / 8;
      field_offset += null_len;
      
      // 更新 null bitmap 字段
      if (new_table_meta.fields_.size() > 0) {
        FieldMeta null_field("__null", AttrType::CHARS, 0, null_len, false, 0, false);
        new_table_meta.fields_[0] = null_field;
      }
      
      // 更新所有字段的偏移量和 ID
      int visible_field_id = 0;
      for (size_t i = new_table_meta.sys_field_num(); i < new_table_meta.fields_.size(); i++) {
        const FieldMeta &old_field = new_table_meta.fields_[i];
        FieldMeta new_field(old_field.name(), old_field.type(), 
                           field_offset,
                           old_field.len(), old_field.visible(), 
                           i, old_field.nullable());
        new_table_meta.fields_[i] = new_field;
        field_offset += old_field.len();
        if (old_field.visible()) {
          visible_field_id++;
        }
      }
      
      new_table_meta.record_size_ = field_offset;
      
      LOG_INFO("Dropping column %s from table %s", old_name.c_str(), name());
      break;
    }
    
    case AlterTableSqlNode::AlterType::RENAME_COLUMN: {
      // 重命名列
      const FieldMeta *old_field = new_table_meta.field(old_name.c_str());
      if (old_field == nullptr) {
        LOG_ERROR("Column does not exist. table=%s, column=%s", name(), old_name.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      
      // 创建新字段（更新名称）
      int field_id = new_table_meta.field_id(old_name.c_str());
      FieldMeta new_field(new_name.c_str(), old_field->type(), old_field->offset(),
                         old_field->len(), old_field->visible(), 
                         old_field->field_id(), old_field->nullable());
      new_table_meta.fields_[field_id] = new_field;
      
      // 更新索引元数据中的字段名
      for (IndexMeta &index : new_table_meta.indexes_) {
        std::vector<std::string> &index_fields = const_cast<std::vector<std::string> &>(index.field());
        for (std::string &index_field : index_fields) {
          if (index_field == old_name) {
            index_field = new_name;
          }
        }
      }
      
      LOG_INFO("Renaming column %s to %s in table %s", old_name.c_str(), new_name.c_str(), name());
      break;
    }
    
    case AlterTableSqlNode::AlterType::CHANGE_COLUMN: {
      // CHANGE COLUMN: 修改列名（类似 RENAME COLUMN，但新名称在 attr_info 中）
      const FieldMeta *old_field = new_table_meta.field(old_name.c_str());
      if (old_field == nullptr) {
        LOG_ERROR("Column does not exist. table=%s, column=%s", name(), old_name.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      
      // 检查新列名是否已存在（且不是旧列名）
      if (attr_info.name != old_name) {
        const FieldMeta *existing_field = new_table_meta.field(attr_info.name.c_str());
        if (existing_field != nullptr) {
          LOG_ERROR("Column already exists. table=%s, column=%s", name(), attr_info.name.c_str());
          return RC::SCHEMA_TABLE_EXIST;
        }
      }
      
      // 创建新字段（更新名称，类型不变）
      int field_id = new_table_meta.field_id(old_name.c_str());
      FieldMeta new_field(attr_info.name.c_str(), old_field->type(), old_field->offset(),
                         old_field->len(), old_field->visible(), 
                         old_field->field_id(), old_field->nullable());
      new_table_meta.fields_[field_id] = new_field;
      
      // 更新索引元数据中的字段名
      for (IndexMeta &index : new_table_meta.indexes_) {
        std::vector<std::string> &index_fields = const_cast<std::vector<std::string> &>(index.field());
        for (std::string &index_field : index_fields) {
          if (index_field == old_name) {
            index_field = attr_info.name;
          }
        }
      }
      
      LOG_INFO("Changing column %s to %s in table %s", old_name.c_str(), attr_info.name.c_str(), name());
      break;
    }
    
    case AlterTableSqlNode::AlterType::RENAME_TABLE: {
      // 重命名表
      new_table_meta.name_ = new_name;
      
      LOG_INFO("Renaming table %s to %s", name(), new_name.c_str());
      break;
    }
    
    default: {
      LOG_ERROR("Unknown alter type. table=%s, alter_type=%d", name(), alter_type);
      return RC::INVALID_ARGUMENT;
    }
  }
  
  // 保存新的元数据到文件
  string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  fstream fs;
  fs.open(tmp_file, ios_base::out | ios_base::binary | ios_base::trunc);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR_OPEN;
  }
  
  if (new_table_meta.serialize(fs) < 0) {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s",
              tmp_file.c_str(), errno, strerror(errno));
    fs.close();
    return RC::IOERR_WRITE;
  }
  fs.close();
  
  // 覆盖原始元数据文件
  string meta_file = table_meta_file(base_dir_.c_str(), name());
  
  int ret = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0) {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s). system error=%d:%s",
              tmp_file.c_str(), meta_file.c_str(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }
  
  // 如果是重命名表，还需要更新数据库中的表名
  if (static_cast<AlterTableSqlNode::AlterType>(alter_type) == AlterTableSqlNode::AlterType::RENAME_TABLE) {
    // 同步重命名元数据文件、数据文件与文本文件
    const std::string old_table_name = name();
    const std::string new_table_name = new_name;

    const std::string old_meta_path = table_meta_file(base_dir_.c_str(), old_table_name.c_str());
    const std::string new_meta_path = table_meta_file(base_dir_.c_str(), new_table_name.c_str());
    if (old_meta_path != new_meta_path) {
      if (::rename(old_meta_path.c_str(), new_meta_path.c_str()) != 0) {
        LOG_ERROR("Failed to rename meta file %s to %s: %s", old_meta_path.c_str(), new_meta_path.c_str(), strerror(errno));
        return RC::IOERR_WRITE;
      }
    }

    const std::string old_data_path = table_data_file(base_dir_.c_str(), old_table_name.c_str());
    const std::string new_data_path = table_data_file(base_dir_.c_str(), new_table_name.c_str());
    if (old_data_path != new_data_path) {
      // best-effort: if data file exists, rename too
      if (::access(old_data_path.c_str(), F_OK) == 0) {
        if (::rename(old_data_path.c_str(), new_data_path.c_str()) != 0) {
          LOG_ERROR("Failed to rename data file %s to %s: %s", old_data_path.c_str(), new_data_path.c_str(), strerror(errno));
          return RC::IOERR_WRITE;
        }
      }
    }

    const std::string old_text_path = table_text_file(base_dir_.c_str(), old_table_name.c_str());
    const std::string new_text_path = table_text_file(base_dir_.c_str(), new_table_name.c_str());
    if (old_text_path != new_text_path) {
      if (::access(old_text_path.c_str(), F_OK) == 0) {
        if (::rename(old_text_path.c_str(), new_text_path.c_str()) != 0) {
          LOG_ERROR("Failed to rename text file %s to %s: %s", old_text_path.c_str(), new_text_path.c_str(), strerror(errno));
          return RC::IOERR_WRITE;
        }
      }
    }
  }
  
  // 更新内存中的元数据
  table_meta_.swap(new_table_meta);
  
  LOG_INFO("Successfully altered table %s", name());
  return RC::SUCCESS;
}

