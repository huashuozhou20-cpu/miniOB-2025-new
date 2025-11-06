#pragma once
#include <cstdio>
#include <cstdarg>
#include <cassert>

#ifndef ASSERT
#define ASSERT(condition, fmt, ...) \
  do { \
    if (!(condition)) { \
      fprintf(stderr, "[ASSERT] " fmt "\n", ##__VA_ARGS__); \
      assert(false); \
    } \
  } while(0)
#endif

