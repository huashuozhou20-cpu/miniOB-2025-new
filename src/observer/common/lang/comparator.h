#pragma once
#include <functional>
#include <cstring>

namespace common {
// Comparator utilities placeholder
template<typename T>
using less = std::less<T>;

template<typename T>
using greater = std::greater<T>;

template<typename T>
using equal_to = std::equal_to<T>;

// Comparison functions
inline int compare_int(const void *a, const void *b) {
  int ia = *(const int*)a;
  int ib = *(const int*)b;
  if (ia < ib) return -1;
  if (ia > ib) return 1;
  return 0;
}

inline int compare_float(const void *a, const void *b) {
  float fa = *(const float*)a;
  float fb = *(const float*)b;
  if (fa < fb) return -1;
  if (fa > fb) return 1;
  return 0;
}

inline int compare_string(const void *a, int len_a, const void *b, int len_b) {
  int min_len = len_a < len_b ? len_a : len_b;
  int cmp = memcmp(a, b, min_len);
  if (cmp != 0) return cmp;
  if (len_a < len_b) return -1;
  if (len_a > len_b) return 1;
  return 0;
}
}

