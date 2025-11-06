#pragma once
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

namespace common {

template<typename T>
class MemPoolSimple {
public:
    MemPoolSimple(const char* name = nullptr) : name_(name ? name : ""), size_(0), capacity_(0), pool_(nullptr) {}
    
    ~MemPoolSimple() {
        destroy();
    }
    
    int init(bool expandable, int initial_capacity) {
        if (pool_ != nullptr) {
            return -1; // Already initialized
        }
        capacity_ = initial_capacity > 0 ? initial_capacity : 100;
        pool_ = static_cast<T*>(malloc(sizeof(T) * capacity_));
        if (pool_ == nullptr) {
            return -1; // Allocation failed
        }
        memset(pool_, 0, sizeof(T) * capacity_);
        size_ = 0;
        expandable_ = expandable;
        return 0;
    }
    
    void destroy() {
        if (pool_ != nullptr) {
            free(pool_);
            pool_ = nullptr;
        }
        size_ = 0;
        capacity_ = 0;
    }
    
    T* alloc() {
        if (size_ >= capacity_) {
            if (!expandable_) {
                return nullptr;
            }
            // Expand capacity
            int new_capacity = capacity_ * 2;
            T* new_pool = static_cast<T*>(realloc(pool_, sizeof(T) * new_capacity));
            if (new_pool == nullptr) {
                return nullptr;
            }
            memset(new_pool + capacity_, 0, sizeof(T) * (new_capacity - capacity_));
            pool_ = new_pool;
            capacity_ = new_capacity;
        }
        T* obj = pool_ + size_;
        size_++;
        return obj;
    }
    
    void free(T* obj) {
        // Simple free - just mark as available
        // In a real implementation, you'd maintain a free list
        // For now, we'll just decrement size if it's the last item
        if (obj >= pool_ && obj < pool_ + size_) {
            if (obj == pool_ + size_ - 1) {
                size_--;
            }
        }
    }
    
    size_t get_size() const {
        return size_;
    }
    
    size_t get_capacity() const {
        return capacity_;
    }
    
private:
    const char* name_;
    T* pool_;
    size_t size_;
    size_t capacity_;
    bool expandable_;
};

// MemPoolItem for variable-size memory allocation
class MemPoolItem {
public:
    explicit MemPoolItem(const char* name = nullptr) : name_(name ? name : "") {}
    
    ~MemPoolItem() {
        for (void* ptr : allocated_) {
            free(ptr);
        }
        allocated_.clear();
    }
    
    // Allocate memory of specified size
    void* alloc(size_t size) {
        void* ptr = malloc(size);
        if (ptr != nullptr) {
            allocated_.push_back(ptr);
        }
        return ptr;
    }
    
    // Allocate and return a unique_ptr-like wrapper
    struct item_unique_ptr {
        item_unique_ptr() : ptr_(nullptr), pool_(nullptr) {}
        item_unique_ptr(void* ptr, MemPoolItem* pool) : ptr_(ptr), pool_(pool) {}
        
        ~item_unique_ptr() {
            // Memory will be freed by MemPoolItem destructor
        }
        
        item_unique_ptr(const item_unique_ptr&) = delete;
        item_unique_ptr& operator=(const item_unique_ptr&) = delete;
        
        item_unique_ptr(item_unique_ptr&& other) noexcept : ptr_(other.ptr_), pool_(other.pool_) {
            other.ptr_ = nullptr;
            other.pool_ = nullptr;
        }
        
        item_unique_ptr& operator=(item_unique_ptr&& other) noexcept {
            if (this != &other) {
                ptr_ = other.ptr_;
                pool_ = other.pool_;
                other.ptr_ = nullptr;
                other.pool_ = nullptr;
            }
            return *this;
        }
        
        void* get() const { return ptr_; }
        void* operator->() const { return ptr_; }
        void* operator*() const { return ptr_; }
        
        explicit operator bool() const { return ptr_ != nullptr; }
        
    private:
        void* ptr_;
        MemPoolItem* pool_;
    };
    
    item_unique_ptr alloc_unique_ptr(size_t size) {
        void* ptr = alloc(size);
        return item_unique_ptr(ptr, this);
    }
    
private:
    const char* name_;
    std::vector<void*> allocated_;
};

} // namespace common
