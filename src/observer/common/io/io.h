/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <unistd.h>
#include <errno.h>

namespace common {

/**
 * @brief Read exactly n bytes from file descriptor
 * @param fd File descriptor
 * @param buf Buffer to store data
 * @param n Number of bytes to read
 * @return 0 on success, -1 on error
 */
inline int readn(int fd, void *buf, size_t n)
{
  char *ptr = static_cast<char *>(buf);
  size_t nleft = n;
  
  while (nleft > 0) {
    ssize_t nread = read(fd, ptr, nleft);
    if (nread < 0) {
      if (errno == EINTR) {
        continue;  // Interrupted, try again
      }
      return -1;  // Error
    } else if (nread == 0) {
      break;  // EOF
    }
    nleft -= nread;
    ptr += nread;
  }
  
  return (n - nleft == n) ? 0 : -1;
}

/**
 * @brief Write exactly n bytes to file descriptor
 * @param fd File descriptor
 * @param buf Buffer containing data
 * @param n Number of bytes to write
 * @return 0 on success, -1 on error
 */
inline int writen(int fd, const void *buf, size_t n)
{
  const char *ptr = static_cast<const char *>(buf);
  size_t nleft = n;
  
  while (nleft > 0) {
    ssize_t nwritten = write(fd, ptr, nleft);
    if (nwritten < 0) {
      if (errno == EINTR) {
        continue;  // Interrupted, try again
      }
      return -1;  // Error
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  
  return 0;
}

}  // namespace common

