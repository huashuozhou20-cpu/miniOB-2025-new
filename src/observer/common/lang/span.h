#pragma once
#include <cstddef>

namespace common {
// Simple span implementation for C++17 compatibility
template<typename T>
class span {
public:
    span() : data_(nullptr), size_(0) {}
    span(T* data, size_t size) : data_(data), size_(size) {}
    span(T* begin, T* end) : data_(begin), size_(end - begin) {}
    
    template<typename Container>
    span(Container& c) : data_(c.data()), size_(c.size()) {}
    
    T* data() const { return data_; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    T& operator[](size_t idx) const { return data_[idx]; }
    T* begin() const { return data_; }
    T* end() const { return data_ + size_; }
    
private:
    T* data_;
    size_t size_;
};
}

using common::span;
