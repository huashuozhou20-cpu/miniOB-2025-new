// src/observer/sql/expr/date_util.h
#pragma once
#include <string>
#include <cstdint>

bool is_leap(int y);
int  days_in_month(int y, int m);

// 严格解析 "YYYY-MM-DD"（允许首尾空白，必须固定 10 位及连字符）
bool parse_yyyy_mm_dd(const std::string &s, int &y, int &m, int &d);

// 计算 0001-01-01 到 (y-1)-12-31 的累计天数（ordinal-1）
int64_t days_before_year(int y);
// ordinal: 从 0001-01-01=1 开始的绝对日序
int64_t ordinal_yyyy_mm_dd(int y, int m, int d);

// 以 1970-01-01 为 0 点，返回相对天数（int32 可覆盖极大范围）
bool date_from_string(const std::string &s, int32_t &out_days);

// 相对天数 -> "YYYY-MM-DD"
void date_to_string(int32_t days, std::string &out);

