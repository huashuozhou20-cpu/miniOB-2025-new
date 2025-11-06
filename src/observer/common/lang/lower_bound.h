#pragma once
#include <algorithm>

namespace common {

template<typename T>
class BinaryIterator {
public:
    BinaryIterator(size_t item_size, T* ptr) : item_size_(item_size), ptr_(ptr) {}
    
    BinaryIterator& operator++() {
        ptr_ += item_size_;
        return *this;
    }
    
    BinaryIterator operator++(int) {
        BinaryIterator tmp = *this;
        ptr_ += item_size_;
        return tmp;
    }
    
    BinaryIterator& operator--() {
        ptr_ -= item_size_;
        return *this;
    }
    
    BinaryIterator operator--(int) {
        BinaryIterator tmp = *this;
        ptr_ -= item_size_;
        return tmp;
    }
    
    BinaryIterator operator+(int n) const {
        return BinaryIterator(item_size_, ptr_ + n * item_size_);
    }
    
    BinaryIterator operator-(int n) const {
        return BinaryIterator(item_size_, ptr_ - n * item_size_);
    }
    
    ptrdiff_t operator-(const BinaryIterator& other) const {
        return (ptr_ - other.ptr_) / item_size_;
    }
    
    T* operator*() const {
        return ptr_;
    }
    
    bool operator==(const BinaryIterator& other) const {
        return ptr_ == other.ptr_;
    }
    
    bool operator!=(const BinaryIterator& other) const {
        return ptr_ != other.ptr_;
    }
    
private:
    size_t item_size_;
    T* ptr_;
};

template<typename Iterator, typename Key, typename Comparator>
Iterator lower_bound(Iterator first, Iterator last, const Key& key, const Comparator& comp, bool* found) {
    Iterator low = first;
    Iterator high = last;
    
    while (low != high) {
        Iterator mid = low + (high - low) / 2;
        int cmp = comp(*mid, key);
        
        if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid;
            if (cmp == 0 && found) {
                *found = true;
            }
        }
    }
    
    if (found && low != last && comp(*low, key) == 0) {
        *found = true;
    } else if (found) {
        *found = false;
    }
    
    return low;
}

} // namespace common

