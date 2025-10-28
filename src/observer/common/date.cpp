// src/observer/common/type/date.cpp    //已改
#include "date.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <limits>

namespace {

// Howard Hinnant 算法，civil <-> days
// 以公历为准，不受 1970/2038 限制

// days from civil (y, m, d), 1970-01-01 为 0
inline int32_t days_from_civil(int y, unsigned m, unsigned d)
{
  y -= (m <= 2);
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + yoe / 400 + doy; // [0, 146096]
  // 1970-01-01 的“天数”：
  // civil_from_days(0) = 1970-01-01，所以这里我们需要让 1970-01-01 → 0
  // era*146097 + doe 为绝对天数，1970-01-01 的绝对天数是 719468
  return static_cast<int32_t>(era * 146097 + static_cast<int>(doe) - 719468);
}

inline void civil_from_days(int32_t z, int &y, unsigned &m, unsigned &d)
{
  // 将相对 1970-01-01 的天数 z 转为绝对天数
  z += 719468;
  const int era = (z >= 0 ? z : z - 146096) / 146097;
  const unsigned doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
  const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
  y = static_cast<int>(yoe) + era * 400;
  const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100 + yoe/400);      // [0, 365]
  const unsigned mp = (5*doy + 2) / 153;                                  // [0, 11]
  d = doy - (153*mp + 2)/5 + 1;                                           // [1, 31]
  m = mp + (mp < 10 ? 3 : -9);                                            // [1, 12]
  y += (m <= 2);
}

inline bool is_leap(int y)
{
  return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

} // anonymous

namespace ob_date {

bool is_valid_ymd(int y, int m, int d)
{
  if (m < 1 || m > 12) return false;
  if (d < 1) return false;
  static const int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
  int dm = mdays[m] + (m == 2 && is_leap(y) ? 1 : 0);
  return d <= dm;
}

RC parse_date_yyyy_mm_dd(const char *s, int32_t &out_days)
{
  if (s == nullptr) return RC::INVALID_ARGUMENT;
  // 允许前后空格，简单跳过
  while (*s == ' ' || *s == '\t') ++s;
  // 期望 YYYY-MM-DD（严格长度 10，且两处 '-'）
  if (std::strlen(s) != 10 || s[4] != '-' || s[7] != '-') return RC::INVALID_ARGUMENT;

  int y = std::atoi(std::string(s, s+4).c_str());
  int m = std::atoi(std::string(s+5, s+7).c_str());
  int d = std::atoi(std::string(s+8, s+10).c_str());

  if (!is_valid_ymd(y, m, d)) return RC::INVALID_ARGUMENT;

  out_days = days_from_civil(y, static_cast<unsigned>(m), static_cast<unsigned>(d));
  return RC::SUCCESS;
}

std::string format_date_yyyy_mm_dd(int32_t days)
{
  int y; unsigned m, d;
  civil_from_days(days, y, m, d);
  char buf[16];
  std::snprintf(buf, sizeof(buf), "%04d-%02u-%02u", y, m, d);
  return std::string(buf);
}

} // namespace ob_date
