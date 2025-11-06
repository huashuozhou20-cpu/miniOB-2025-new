#pragma once
#include <list>
#include <unordered_map>

namespace common {
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class LRUCache {
public:
    LRUCache() : capacity_(0) {}
    LRUCache(size_t capacity) : capacity_(capacity) {}
    
    void put(const Key& key, const Value& value) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }
        
        if (items_.size() >= capacity_) {
            auto last = items_.back();
            cache_.erase(last.first);
            items_.pop_back();
        }
        
        items_.emplace_front(key, value);
        cache_[key] = items_.begin();
    }
    
    bool get(const Key& key, Value& value) {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return false;
        }
        value = it->second->second;
        items_.splice(items_.begin(), items_, it->second);
        return true;
    }
    
    size_t count() const {
        return items_.size();
    }
    
    void destroy() {
        items_.clear();
        cache_.clear();
    }
    
    void init(size_t capacity) {
        capacity_ = capacity;
        items_.clear();
        cache_.clear();
    }
    
    template<typename Func>
    void foreach_reverse(Func&& func) {
        for (auto it = items_.rbegin(); it != items_.rend(); ++it) {
            if (!func(it->first, it->second)) {
                break;
            }
        }
    }
    
    template<typename Func>
    void foreach(Func&& func) {
        for (auto it = items_.begin(); it != items_.end(); ++it) {
            if (!func(it->first, it->second)) {
                break;
            }
        }
    }
    
    bool remove(const Key& key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return false;
        }
        items_.erase(it->second);
        cache_.erase(it);
        return true;
    }
    
private:
    size_t capacity_;
    std::list<std::pair<Key, Value>> items_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator, Hash> cache_;
};
}
