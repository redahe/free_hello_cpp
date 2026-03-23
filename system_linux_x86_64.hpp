/*
 *  Implementation of system call wrappers for Linux x86_64
 */

#pragma once

#include <cpuid.h> //For __get_cpuid
#include <cstdint>
#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

#if defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE() __builtin_unreachable();
#endif

namespace _system {

[[noreturn]] inline void crash() {
  // This will result in "Illegal instruction (core dumped)" on Linux
  asm volatile("ud2");
  UNREACHABLE();
}

inline bool has_rdrand_support() {
  unsigned int eax, ebx, ecx, edx;
  if (__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
    if (ecx & (1 << 30)) {
      return true;
    }
  }
  return false;
}

inline uintptr_t get_rdrand_value() {
  uintptr_t val;
  unsigned char succeed;
  do {
    __asm__ __volatile__("rdrand %0; setc %1"
                         : "=r"(val), "=qm"(succeed)
                         :
                         : "cc" // Informs the compiler that the condition code
                                // (flags) are modified
    );
  } while (!succeed);
  return val;
}

[[noreturn]] inline void exit(const int status) {
  asm volatile("syscall"
               :
               : "a"(60), "D"((long)(status & 255))
               : "rcx", "r11", "memory");
  UNREACHABLE();
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
