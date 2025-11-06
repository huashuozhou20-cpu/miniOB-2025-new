#pragma once
#include <atomic>

namespace common {

// Type alias for std::atomic
template<typename T>
using atomic = std::atomic<T>;

using atomic_bool = std::atomic<bool>;
using atomic_int = std::atomic<int>;
using atomic_int64_t = std::atomic<int64_t>;

} // namespace common
