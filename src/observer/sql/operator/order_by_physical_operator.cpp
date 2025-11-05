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

#include <queue>
#include <fstream>
#include <filesystem>
#include <random>
#include <sstream>
#include <iomanip>

#include "sql/operator/order_by_physical_operator.h"
#include "common/log/log.h"

using namespace std;
using namespace filesystem;

OrderByPhysicalOperator::OrderByPhysicalOperator(std::vector<std::unique_ptr<Expression>>&& order_by, std::vector<bool>&& is_asc, int limit)
    : order_by_(move(order_by)), is_asc_(move(is_asc)), limit_(limit){}

RC OrderByPhysicalOperator::open(Trx *trx)
{
    LOG_INFO("open order by operator");
    if (children_.empty()) {
        return RC::SUCCESS;
    }

    std::unique_ptr<PhysicalOperator> &child = children_[0];

    RC rc = child->open(trx);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to open child operator: %s", strrc(rc));
        return rc;
    }

    have_value = false;
    value_list_.clear();
    ids_.clear();
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next()
{
    RC                rc    = RC::SUCCESS;
    if(!have_value){
        if(limit_ < 0) {
            // Check if we need external sort
            // First, try to collect a sample to estimate size
            rc = quick_sort();
            // If quick_sort failed due to memory, fall back to external sort
            if (OB_FAIL(rc) && rc == RC::INVALID_ARGUMENT) {
                // Reset and try external sort
                value_list_.clear();
                order_values_.clear();
                ids_.clear();
                rc = external_sort();
            }
        } else {
            rc = limit_sort();
        }

        if(OB_FAIL(rc))return rc;

        current_id_ = 0;
        first_emited_  = false;
        have_value = true;
    }

    return fetch_next();
}

RC OrderByPhysicalOperator::close()
{
    LOG_INFO("close order by operator");
    // Cleanup temporary files
    if (!external_sort_file_.empty()) {
        cleanup_temp_files({external_sort_file_});
        external_sort_file_.clear();
    }
    cleanup_temp_files(temp_files_);
    temp_files_.clear();
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next(Tuple *upper_tuple) 
{
    RC                rc    = RC::SUCCESS;
    if(!have_value){
        if(limit_ < 0) {
            // Check if we need external sort
            rc = quick_sort(upper_tuple);
            // If quick_sort failed due to memory, fall back to external sort
            if (OB_FAIL(rc) && rc == RC::INVALID_ARGUMENT) {
                // Reset and try external sort
                value_list_.clear();
                order_values_.clear();
                ids_.clear();
                rc = external_sort(upper_tuple);
            }
        } else {
            rc = limit_sort(upper_tuple);
        }

        if(OB_FAIL(rc))return rc;

        current_id_ = 0;
        first_emited_  = false;
        have_value = true;
    }

    return fetch_next();
}

Tuple *OrderByPhysicalOperator::current_tuple()
{
  if (current_id_ != value_list_.size()) {
    return &value_list_[ids_[current_id_]];
  }
  return nullptr;
}

RC OrderByPhysicalOperator::fetch_next()
{
  if (current_id_ == value_list_.size()) {
    return RC::RECORD_EOF;
  }

  if (first_emited_) {
    ++current_id_;
  } else {
    first_emited_ = true;
  }
  if (current_id_ == value_list_.size()) {
    return RC::RECORD_EOF;
  }

  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::quick_sort(Tuple *upper_tuple)
{
    RC rc = RC::SUCCESS;
    std::unique_ptr<PhysicalOperator> &child = children_[0];
    Tuple* tuple = nullptr;
    size_t count = 0;
    
    while (OB_SUCC(rc = (upper_tuple == nullptr ? child->next() : child->next(upper_tuple)))) {
        tuple = child->current_tuple();
        if (nullptr == tuple) {
            LOG_WARN("failed to get current record: %s", strrc(rc));
            child->close();
            return rc;
        }
        ValueListTuple value_list;
        rc =  ValueListTuple::make(*tuple, value_list);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to make ValueListTuple: %s", strrc(rc));
            child->close();
            return rc;
        }
        value_list_.emplace_back(std::move(value_list));
        count++;
        
        // If we exceed memory threshold, switch to external sort
        if (count > MEMORY_THRESHOLD) {
            child->close();
            LOG_INFO("Data size exceeds memory threshold, switching to external sort");
            // Return INVALID_ARGUMENT to signal that we should use external sort
            return RC::INVALID_ARGUMENT;
        }
    }
    child->close();

    if (RC::RECORD_EOF == rc) {
        rc = RC::SUCCESS;
    }

    if (OB_FAIL(rc)) {
        LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
        return rc;
    }

    ids_.resize(value_list_.size());
    order_values_.resize(value_list_.size());
    for(size_t id = 0; id < value_list_.size(); id++){
        auto& values = order_values_[id];
        values.resize(order_by_.size());
        for(size_t i = 0; i < order_by_.size(); i++){
            order_by_[i]->get_value(value_list_[id], values[i]);
        }
        ids_[id] = id;
    }

    sort(ids_.begin(), ids_.end(), [&](size_t &a, size_t &b){
        return cmp(order_values_[a], order_values_[b]);
    });

    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::limit_sort(Tuple *upper_tuple)
{
    RC rc = RC::SUCCESS;
    std::unique_ptr<PhysicalOperator> &child = children_[0];
    Tuple* tuple = nullptr;
    order_values_.reserve(limit_);
    value_list_.reserve(limit_);

    auto cmp_ = [&](size_t &a, size_t &b){
        return cmp(order_values_[a], order_values_[b]);
    };
    priority_queue<size_t, vector<size_t>, decltype(cmp_)> pq(cmp_);
    
    while (OB_SUCC(rc = (upper_tuple == nullptr ? child->next() : child->next(upper_tuple)))) {
        tuple = child->current_tuple();
        if (nullptr == tuple) {
            LOG_WARN("failed to get current record: %s", strrc(rc));
            child->close();
            return rc;
        }
        ValueListTuple value_list;
        rc =  ValueListTuple::make(*tuple, value_list);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to make ValueListTuple: %s", strrc(rc));
            child->close();
            return rc;
        }
        if(pq.size() < (size_t)limit_)
        {
            vector<Value> values(order_by_.size());
            for(size_t i = 0; i < order_by_.size(); i++){
                order_by_[i]->get_value(value_list, values[i]);
            }
            value_list_.emplace_back(std::move(value_list));
            order_values_.emplace_back(std::move(values));
            pq.emplace(value_list_.size() - 1);
        } else {
            vector<Value> values(order_by_.size());
            for(size_t i = 0; i < order_by_.size(); i++){
                order_by_[i]->get_value(value_list, values[i]);
            }
            if(cmp(values, order_values_[pq.top()])){
                size_t id = pq.top();
                pq.pop();
                order_values_[id].swap(values);
                pq.emplace(id);
                value_list_[id] = move(value_list);
            }
        }
            
    }
    child->close();

    if (RC::RECORD_EOF == rc) {
        rc = RC::SUCCESS;
    }

    if (OB_FAIL(rc)) {
        LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
        return rc;
    }

    ids_.reserve(limit_);
    while(!pq.empty()){
        ids_.emplace_back(pq.top());
        pq.pop();
    }
    reverse(ids_.begin(), ids_.end());

    return RC::SUCCESS;
}

bool OrderByPhysicalOperator::cmp(const vector<Value>& a_vals, const vector<Value>& b_vals)
{
    for(size_t id = 0; id < order_by_.size(); id++){
        bool is_asc = is_asc_[id];
        auto& a_val = a_vals[id];
        auto& b_val = b_vals[id];

        if(a_val.attr_type() == AttrType::NULLS)
        {
            if(b_val.attr_type() == AttrType::NULLS)continue;
            return is_asc;
        }
        else if(b_val.attr_type() == AttrType::NULLS)return !is_asc;

        int cmp = a_val.compare(b_val);
        if(cmp == 0)continue;
        else if(is_asc)return cmp < 0;
        return cmp > 0;
    }
    return false;
}

RC OrderByPhysicalOperator::external_sort(Tuple *upper_tuple)
{
    RC rc = RC::SUCCESS;
    std::unique_ptr<PhysicalOperator> &child = children_[0];
    Tuple* tuple = nullptr;
    
    // Step 1: Read data in chunks and sort each chunk
    vector<string> chunk_files;
    vector<ValueListTuple> chunk;
    vector<vector<Value>> chunk_values;
    vector<size_t> chunk_ids;
    size_t chunk_count = 0;
    size_t total_count = 0;
    
    // Generate temp directory path
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999999);
    stringstream ss;
    ss << "/tmp/order_by_" << dis(gen) << "_";
    string temp_prefix = ss.str();
    
    while (OB_SUCC(rc = (upper_tuple == nullptr ? child->next() : child->next(upper_tuple)))) {
        tuple = child->current_tuple();
        if (nullptr == tuple) {
            LOG_WARN("failed to get current record: %s", strrc(rc));
            child->close();
            cleanup_temp_files(chunk_files);
            return rc;
        }
        
        ValueListTuple value_list;
        rc = ValueListTuple::make(*tuple, value_list);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to make ValueListTuple: %s", strrc(rc));
            child->close();
            cleanup_temp_files(chunk_files);
            return rc;
        }
        
        vector<Value> values(order_by_.size());
        for(size_t i = 0; i < order_by_.size(); i++){
            order_by_[i]->get_value(value_list, values[i]);
        }
        
        chunk.push_back(std::move(value_list));
        chunk_values.push_back(std::move(values));
        chunk_ids.push_back(chunk.size() - 1);
        total_count++;
        
        // When chunk is full, sort and write to file
        if (chunk.size() >= CHUNK_SIZE) {
            // Sort the chunk
            sort(chunk_ids.begin(), chunk_ids.end(), [&](size_t a, size_t b){
                return cmp(chunk_values[a], chunk_values[b]);
            });
            
            // Write chunk to file
            string chunk_file = temp_prefix + to_string(chunk_count++) + ".chunk";
            rc = write_chunk_to_file(chunk, chunk_values, chunk_ids, chunk_file);
            if (OB_FAIL(rc)) {
                child->close();
                cleanup_temp_files(chunk_files);
                return rc;
            }
            chunk_files.push_back(chunk_file);
            
            // Clear chunk for next batch
            chunk.clear();
            chunk_values.clear();
            chunk_ids.clear();
        }
    }
    child->close();
    
    if (RC::RECORD_EOF != rc && OB_FAIL(rc)) {
        LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
        cleanup_temp_files(chunk_files);
        return rc;
    }
    
    // Write remaining data as last chunk
    if (!chunk.empty()) {
        sort(chunk_ids.begin(), chunk_ids.end(), [&](size_t a, size_t b){
            return cmp(chunk_values[a], chunk_values[b]);
        });
        
        string chunk_file = temp_prefix + to_string(chunk_count++) + ".chunk";
        rc = write_chunk_to_file(chunk, chunk_values, chunk_ids, chunk_file);
        if (OB_FAIL(rc)) {
            cleanup_temp_files(chunk_files);
            return rc;
        }
        chunk_files.push_back(chunk_file);
    }
    
    // Step 2: Merge all sorted chunks
    if (chunk_files.empty()) {
        return RC::SUCCESS;
    }
    
    external_sort_file_ = temp_prefix + "final.sorted";
    rc = merge_sorted_chunks(chunk_files, external_sort_file_);
    if (OB_FAIL(rc)) {
        cleanup_temp_files(chunk_files);
        cleanup_temp_files({external_sort_file_});
        return rc;
    }
    
    // Step 3: Load merged result into memory
    rc = read_chunk_from_file(external_sort_file_, value_list_, order_values_, ids_);
    if (OB_FAIL(rc)) {
        cleanup_temp_files(chunk_files);
        cleanup_temp_files({external_sort_file_});
        return rc;
    }
    
    // Cleanup intermediate chunk files
    cleanup_temp_files(chunk_files);
    temp_files_.push_back(external_sort_file_);
    
    LOG_INFO("External sort completed. Total records: %zu, Chunks: %zu", total_count, chunk_files.size());
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::write_chunk_to_file(const vector<ValueListTuple> &chunk, 
                                                 const vector<vector<Value>> &chunk_values,
                                                 const vector<size_t> &chunk_ids,
                                                 const string &filename)
{
    ofstream fs(filename, ios::binary | ios::out);
    if (!fs.is_open()) {
        LOG_ERROR("Failed to open file for write: %s", filename.c_str());
        return RC::IOERR_OPEN;
    }
    
    // Write chunk size
    size_t chunk_size = chunk_ids.size();
    fs.write(reinterpret_cast<const char*>(&chunk_size), sizeof(chunk_size));
    
    // Write each tuple in sorted order
    for (size_t id : chunk_ids) {
        const ValueListTuple &tuple = chunk[id];
        const vector<Value> &values = chunk_values[id];
        
        // Write number of cells
        int cell_num = tuple.cell_num();
        fs.write(reinterpret_cast<const char*>(&cell_num), sizeof(cell_num));
        
        // Write each cell as string (simplified serialization)
        for (int i = 0; i < cell_num; i++) {
            Value cell;
            if (tuple.cell_at(i, cell) != RC::SUCCESS) {
                fs.close();
                return RC::INVALID_ARGUMENT;
            }
            
            string cell_str = cell.to_string();
            size_t str_len = cell_str.length();
            fs.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
            fs.write(cell_str.c_str(), str_len);
        }
        
        // Write order values
        size_t order_size = values.size();
        fs.write(reinterpret_cast<const char*>(&order_size), sizeof(order_size));
        for (const Value &val : values) {
            string val_str = val.to_string();
            size_t str_len = val_str.length();
            fs.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
            fs.write(val_str.c_str(), str_len);
        }
    }
    
    fs.close();
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::read_chunk_from_file(const string &filename,
                                                  vector<ValueListTuple> &chunk,
                                                  vector<vector<Value>> &chunk_values,
                                                  vector<size_t> &chunk_ids)
{
    ifstream fs(filename, ios::binary | ios::in);
    if (!fs.is_open()) {
        LOG_ERROR("Failed to open file for read: %s", filename.c_str());
        return RC::IOERR_OPEN;
    }
    
    // Read chunk size
    size_t chunk_size;
    fs.read(reinterpret_cast<char*>(&chunk_size), sizeof(chunk_size));
    if (!fs.good()) {
        fs.close();
        return RC::IOERR_READ;
    }
    
    chunk.reserve(chunk_size);
    chunk_values.reserve(chunk_size);
    chunk_ids.reserve(chunk_size);
    
    // Read each tuple
    for (size_t i = 0; i < chunk_size; i++) {
        // Read number of cells
        int cell_num;
        fs.read(reinterpret_cast<char*>(&cell_num), sizeof(cell_num));
        if (!fs.good()) {
            fs.close();
            return RC::IOERR_READ;
        }
        
        ValueListTuple tuple;
        vector<Value> cells;
        vector<TupleCellSpec> specs;
        
        // Read each cell
        for (int j = 0; j < cell_num; j++) {
            size_t str_len;
            fs.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
            if (!fs.good()) {
                fs.close();
                return RC::IOERR_READ;
            }
            
            string cell_str(str_len, '\0');
            fs.read(&cell_str[0], str_len);
            if (!fs.good()) {
                fs.close();
                return RC::IOERR_READ;
            }
            
            // Parse value from string (simplified)
            Value cell;
            // Note: This is a simplified parser. In production, you'd want proper deserialization
            // For now, we'll try to parse as different types
            if (cell_str == "null" || cell_str == "NULL" || cell_str.empty()) {
                cell = Value((void*)nullptr);
            } else {
                // Try to parse as int
                try {
                    int int_val = stoi(cell_str);
                    cell = Value(int_val);
                } catch (...) {
                    // Try float
                    try {
                        float float_val = stof(cell_str);
                        cell = Value(float_val);
                    } catch (...) {
                        // Default to string
                        cell = Value(cell_str.c_str(), cell_str.length());
                    }
                }
            }
            
            cells.push_back(cell);
            // Create a simple spec
            string spec_name = "col_" + to_string(j);
            specs.push_back(TupleCellSpec(spec_name.c_str()));
        }
        
        tuple.set_cells(cells);
        tuple.set_names(specs);
        chunk.push_back(tuple);
        
        // Read order values
        size_t order_size;
        fs.read(reinterpret_cast<char*>(&order_size), sizeof(order_size));
        if (!fs.good()) {
            fs.close();
            return RC::IOERR_READ;
        }
        
        vector<Value> order_vals;
        for (size_t k = 0; k < order_size; k++) {
            size_t str_len;
            fs.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
            if (!fs.good()) {
                fs.close();
                return RC::IOERR_READ;
            }
            
            string val_str(str_len, '\0');
            fs.read(&val_str[0], str_len);
            if (!fs.good()) {
                fs.close();
                return RC::IOERR_READ;
            }
            
            Value val;
            if (val_str == "null" || val_str == "NULL" || val_str.empty()) {
                val = Value((void*)nullptr);
            } else {
                try {
                    int int_val = stoi(val_str);
                    val = Value(int_val);
                } catch (...) {
                    try {
                        float float_val = stof(val_str);
                        val = Value(float_val);
                    } catch (...) {
                        val = Value(val_str.c_str(), val_str.length());
                    }
                }
            }
            order_vals.push_back(val);
        }
        chunk_values.push_back(order_vals);
        chunk_ids.push_back(i);
    }
    
    fs.close();
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::merge_sorted_chunks(const vector<string> &chunk_files, const string &output_file)
{
    if (chunk_files.empty()) {
        return RC::INVALID_ARGUMENT;
    }
    
    if (chunk_files.size() == 1) {
        // Only one chunk, just copy it
        ifstream src(chunk_files[0], ios::binary);
        ofstream dst(output_file, ios::binary);
        if (!src.is_open() || !dst.is_open()) {
            return RC::IOERR_OPEN;
        }
        dst << src.rdbuf();
        src.close();
        dst.close();
        return RC::SUCCESS;
    }
    
    // Multi-way merge: open all chunk files and merge them
    vector<ifstream> files;
    vector<ValueListTuple> current_tuples;
    vector<vector<Value>> current_values;
    vector<bool> file_eof;
    
    for (const string &file : chunk_files) {
        files.emplace_back(file, ios::binary);
        if (!files.back().is_open()) {
            for (auto &f : files) {
                if (f.is_open()) f.close();
            }
            return RC::IOERR_OPEN;
        }
        
        // Read first tuple from each file
        ValueListTuple first_tuple;
        vector<Value> first_values;
        RC rc = read_next_tuple_from_file(files.back(), first_tuple, first_values);
        if (rc == RC::SUCCESS) {
            current_tuples.push_back(first_tuple);
            current_values.push_back(first_values);
            file_eof.push_back(false);
        } else if (rc == RC::RECORD_EOF) {
            file_eof.push_back(true);
            current_tuples.push_back(ValueListTuple());
            current_values.push_back(vector<Value>());
        } else {
            for (auto &f : files) {
                if (f.is_open()) f.close();
            }
            return rc;
        }
    }
    
    // Merge and write to output
    ofstream out(output_file, ios::binary);
    if (!out.is_open()) {
        for (auto &f : files) {
            if (f.is_open()) f.close();
        }
        return RC::IOERR_OPEN;
    }
    
    // Write total size (will update later)
    size_t total_size = 0;
    streampos size_pos = out.tellp();
    out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));
    
    // Multi-way merge
    while (true) {
        // Find the minimum tuple among all active files
        int min_idx = -1;
        for (size_t i = 0; i < chunk_files.size(); i++) {
            if (!file_eof[i]) {
                if (min_idx == -1 || cmp(current_values[i], current_values[min_idx])) {
                    min_idx = i;
                }
            }
        }
        
        if (min_idx == -1) {
            break;  // All files exhausted
        }
        
        // Write the minimum tuple
        RC rc = write_tuple_to_file(out, current_tuples[min_idx], current_values[min_idx]);
        if (OB_FAIL(rc)) {
            for (auto &f : files) {
                if (f.is_open()) f.close();
            }
            out.close();
            return rc;
        }
        total_size++;
        
        // Read next tuple from the file we just consumed
        rc = read_next_tuple_from_file(files[min_idx], current_tuples[min_idx], current_values[min_idx]);
        if (rc == RC::RECORD_EOF) {
            file_eof[min_idx] = true;
        } else if (OB_FAIL(rc)) {
            for (auto &f : files) {
                if (f.is_open()) f.close();
            }
            out.close();
            return rc;
        }
    }
    
    // Update total size
    out.seekp(size_pos);
    out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));
    
    for (auto &f : files) {
        if (f.is_open()) f.close();
    }
    out.close();
    
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::read_next_tuple_from_file(ifstream &fs, ValueListTuple &tuple, vector<Value> &values)
{
    if (!fs.good()) {
        return RC::RECORD_EOF;
    }
    
    // Read number of cells
    int cell_num;
    fs.read(reinterpret_cast<char*>(&cell_num), sizeof(cell_num));
    if (!fs.good() || fs.eof()) {
        return RC::RECORD_EOF;
    }
    
    vector<Value> cells;
    vector<TupleCellSpec> specs;
    
    // Read each cell
    for (int i = 0; i < cell_num; i++) {
        size_t str_len;
        fs.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
        if (!fs.good()) {
            return RC::IOERR_READ;
        }
        
        string cell_str(str_len, '\0');
        fs.read(&cell_str[0], str_len);
        if (!fs.good()) {
            return RC::IOERR_READ;
        }
        
        Value cell;
        if (cell_str == "null" || cell_str == "NULL" || cell_str.empty()) {
            cell = Value((void*)nullptr);
        } else {
            try {
                int int_val = stoi(cell_str);
                cell = Value(int_val);
            } catch (...) {
                try {
                    float float_val = stof(cell_str);
                    cell = Value(float_val);
                } catch (...) {
                    cell = Value(cell_str.c_str(), cell_str.length());
                }
            }
        }
        cells.push_back(cell);
        
        string spec_name = "col_" + to_string(i);
        specs.push_back(TupleCellSpec(spec_name.c_str()));
    }
    
    // Read order values
    size_t order_size;
    fs.read(reinterpret_cast<char*>(&order_size), sizeof(order_size));
    if (!fs.good()) {
        return RC::IOERR_READ;
    }
    
    values.clear();
    for (size_t i = 0; i < order_size; i++) {
        size_t str_len;
        fs.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));
        if (!fs.good()) {
            return RC::IOERR_READ;
        }
        
        string val_str(str_len, '\0');
        fs.read(&val_str[0], str_len);
        if (!fs.good()) {
            return RC::IOERR_READ;
        }
        
        Value val;
        if (val_str == "null" || val_str == "NULL" || val_str.empty()) {
            val = Value((void*)nullptr);
        } else {
            try {
                int int_val = stoi(val_str);
                val = Value(int_val);
            } catch (...) {
                try {
                    float float_val = stof(val_str);
                    val = Value(float_val);
                } catch (...) {
                    val = Value(val_str.c_str(), val_str.length());
                }
            }
        }
        values.push_back(val);
    }
    
    tuple.set_cells(cells);
    tuple.set_names(specs);
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::write_tuple_to_file(ofstream &fs, const ValueListTuple &tuple, const vector<Value> &values)
{
    // Write number of cells
    int cell_num = tuple.cell_num();
    fs.write(reinterpret_cast<const char*>(&cell_num), sizeof(cell_num));
    
    // Write each cell
    for (int i = 0; i < cell_num; i++) {
        Value cell;
        if (tuple.cell_at(i, cell) != RC::SUCCESS) {
            return RC::INVALID_ARGUMENT;
        }
        
        string cell_str = cell.to_string();
        size_t str_len = cell_str.length();
        fs.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
        fs.write(cell_str.c_str(), str_len);
    }
    
    // Write order values
    size_t order_size = values.size();
    fs.write(reinterpret_cast<const char*>(&order_size), sizeof(order_size));
    for (const Value &val : values) {
        string val_str = val.to_string();
        size_t str_len = val_str.length();
        fs.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
        fs.write(val_str.c_str(), str_len);
    }
    
    return RC::SUCCESS;
}

void OrderByPhysicalOperator::cleanup_temp_files(const vector<string> &files)
{
    for (const string &file : files) {
        try {
            if (exists(file)) {
                remove(file);
            }
        } catch (...) {
            // Ignore errors during cleanup
        }
    }
}
