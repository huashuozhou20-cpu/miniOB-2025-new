#pragma once
#include <vector>
#include <cstring>

namespace common {

class Bitmap {
public:
    Bitmap() : bits_(0, false) {}
    
    Bitmap(char* data, int size) {
        bits_.resize(size, false);
        if (data != nullptr) {
            for (int i = 0; i < size; i++) {
                bits_[i] = (data[i / 8] & (1 << (i % 8))) != 0;
            }
        }
    }
    
    Bitmap(size_t size) : bits_(size, false) {}
    
    void init(char* data, int size) {
        bits_.resize(size, false);
        if (data != nullptr) {
            for (int i = 0; i < size; i++) {
                bits_[i] = (data[i / 8] & (1 << (i % 8))) != 0;
            }
        }
    }
    
    void set_bit(size_t index) {
        if (index < bits_.size()) {
            bits_[index] = true;
        }
    }
    
    void clear_bit(size_t index) {
        if (index < bits_.size()) {
            bits_[index] = false;
        }
    }
    
    bool get_bit(size_t index) const {
        return index < bits_.size() ? bits_[index] : false;
    }
    
    int next_setted_bit(size_t start_index) const {
        for (size_t i = start_index; i < bits_.size(); i++) {
            if (bits_[i]) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    int next_unsetted_bit(size_t start_index) const {
        for (size_t i = start_index; i < bits_.size(); i++) {
            if (!bits_[i]) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    size_t size() const {
        return bits_.size();
    }
    
private:
    std::vector<bool> bits_;
};

}

