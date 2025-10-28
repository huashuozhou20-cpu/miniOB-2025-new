// src/observer/common/type/date.h
#pragma once
#include <string>
#include "common/rc.h"

namespace ob_date {

// 解析 "YYYY-MM-DD" → days(1970-01-01 为 0，之前为负数)
RC parse_date_yyyy_mm_dd(const char *s, int32_t &out_days);

// 格式化 days → "YYYY-MM-DD"
std::string format_date_yyyy_mm_dd(int32_t days);

// 校验：是否是合法日期
bool is_valid_ymd(int y, int m, int d);

} // namespace ob_date
