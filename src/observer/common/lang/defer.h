#pragma once

#include <functional>

namespace common {

// RAII helper for defer execution
template<typename Func>
class DeferGuard {
public:
    explicit DeferGuard(Func&& func) : func_(std::forward<Func>(func)) {}
    ~DeferGuard() { func_(); }
    
    DeferGuard(const DeferGuard&) = delete;
    DeferGuard& operator=(const DeferGuard&) = delete;
    DeferGuard(DeferGuard&&) = delete;
    DeferGuard& operator=(DeferGuard&&) = delete;

private:
    Func func_;
};

// Helper function to create DeferGuard
template<typename Func>
DeferGuard<Func> make_defer(Func&& func) {
    return DeferGuard<Func>(std::forward<Func>(func));
}

} // namespace common

// Macro for defer execution
#define DEFER(code) \
    auto __defer_guard_##__LINE__ = common::make_defer([&]() { code; })

