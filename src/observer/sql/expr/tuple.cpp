#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>           // for std::min
#include "sql/expr/tuple_cell.h"
#include "common/lang/string.h"
#include "common/date.h"  // 用于日期解析/格式化
using namespace std;

TupleCellSpec::TupleCellSpec(const char *table_name, const char *field_name, const char *alias)
{
  if (table_name) {
    table_name_ = table_name;
  }
  if (field_name) {
    field_name_ = field_name;
  }
  if (alias) {
    alias_ = alias;
  } else {
    if (table_name_.empty()) {
      alias_ = field_name_;
    } else {
      alias_ = table_name_ + "." + field_name_;
    }
  }
}

TupleCellSpec::TupleCellSpec(const char *alias)
{
  if (alias) {
    alias_ = alias;
  }
}

TupleCellSpec::TupleCellSpec(const string &alias) : alias_(alias)
{}

// ===================== TupleCell =====================
void TupleCell::to_string(std::string &out) const
{
  const void *p = data();
  const int   n = length();
  if (p == nullptr || n <= 0) {
    out.clear();
    return;
  }

  switch (attr_type()) {
    case AttrType::INTS: {
      int32_t v = 0;
      memcpy(&v, p, sizeof(v));
      out = std::to_string(v);
    } break;

    case AttrType::FLOATS: {
      float v = 0.0f;
      memcpy(&v, p, sizeof(v));
      char buf[64];
      std::snprintf(buf, sizeof(buf), "%g", static_cast<double>(v));
      out = buf;
    } break;

    case AttrType::CHARS: {
      out.assign(static_cast<const char *>(p), n);
      // 去掉可能的尾部 '\0'
      if (!out.empty() && out.back() == '\0') {
        out.pop_back();
      }
    } break;

    case AttrType::DATES: {
      // 以“自纪元起的天数(int32_t)”存储
      int32_t days = 0;
      memcpy(&days, p, sizeof(days));
      out = ob_date::format_date_yyyy_mm_dd(days);
      printf("DATE cell days=%d", days);
    } break;

    default: {
      out.clear();
    } break;
  }
}

static inline int cmp_int32(int32_t a, int32_t b)
{
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

int TupleCell::compare(const TupleCell &other) const
{
  const AttrType t1 = attr_type();
  const AttrType t2 = other.attr_type();

  // 数字互比：INT<->INT / FLOAT<->FLOAT / INT<->FLOAT
  auto is_numeric = [](AttrType t) {
    return t == AttrType::INTS || t == AttrType::FLOATS;
  };
  if (is_numeric(t1) && is_numeric(t2)) {
    double a = 0.0, b = 0.0;
    if (t1 == AttrType::INTS)  { int32_t v=0; memcpy(&v, data(), sizeof(v));  a = static_cast<double>(v); }
    else                       { float   v=0; memcpy(&v, data(), sizeof(v));  a = static_cast<double>(v); }

    if (t2 == AttrType::INTS)  { int32_t v=0; memcpy(&v, other.data(), sizeof(v)); b = static_cast<double>(v); }
    else                       { float   v=0; memcpy(&v, other.data(), sizeof(v)); b = static_cast<double>(v); }

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
  }

  // 日期互比：DATES<->DATES
  if (t1 == AttrType::DATES && t2 == AttrType::DATES) {
    int32_t a=0, b=0;
    memcpy(&a, data(), sizeof(a));
    memcpy(&b, other.data(), sizeof(b));
    return cmp_int32(a, b);
  }

  // 日期 vs 字符串：尝试把字符串解析成 days 再比；解析失败则回退到字节序比较
  if ((t1 == AttrType::DATES && t2 == AttrType::CHARS) ||
      (t2 == AttrType::DATES && t1 == AttrType::CHARS)) {
    int32_t a_days = 0, b_days = 0;
    bool ok1 = false, ok2 = false;

    if (t1 == AttrType::DATES) {
      memcpy(&a_days, data(), sizeof(a_days));
      ok1 = true;
    } else { // t1 == CHARS
      const char *p1 = static_cast<const char *>(data());
      std::string s1(p1, length());
      if (!s1.empty() && s1.back() == '\0') s1.pop_back();
      RC rc = ob_date::parse_date_yyyy_mm_dd(s1.c_str(), a_days);
      ok1 = (rc == RC::SUCCESS);
    }

    if (t2 == AttrType::DATES) {
      memcpy(&b_days, other.data(), sizeof(b_days));
      ok2 = true;
    } else { // t2 == CHARS
      const char *p2 = static_cast<const char *>(other.data());
      std::string s2(p2, other.length());
      if (!s2.empty() && s2.back() == '\0') s2.pop_back();
      RC rc = ob_date::parse_date_yyyy_mm_dd(s2.c_str(), b_days);
      ok2 = (rc == RC::SUCCESS);
    }

    if (ok1 && ok2) {
      return cmp_int32(a_days, b_days);
    }
    // 回退按字节序比较
  }

  // 字符串互比 或 异构兜底：按字节序比较（再用长度决胜负）
  {
    const int len1 = length();
    const int len2 = other.length();
    const int minlen = std::min(len1, len2);
    const char *s1 = static_cast<const char *>(data());
    const char *s2 = static_cast<const char *>(other.data());
    const int r = std::memcmp(s1, s2, minlen);
    if (r < 0) return -1;
    if (r > 0) return 1;
    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    return 0;
  }
}
