/*
 *  Implementation of system call wrappers for Linux x86_64
 */

#pragma once

#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

namespace _system {

inline void exit(const int status) {
  asm volatile("syscall"
               :
               : "a"(60), "D"((long)(status & 255))
               : "rcx", "r11", "memory");
}

[[nodiscard]] inline ssize_t write(const int fd, std::string_view str) {
  ssize_t ret;
  asm volatile("syscall"
               : "=a"(ret)
               : "0"(1), "D"((long)fd), "S"(str.data()), "d"(str.size())
               : "rcx", "r11", "memory");
  return ret;
}

} // namespace _system
