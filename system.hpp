/*
 *  System call wrappers, and low level necessities.
 */

#pragma once

#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

int main();

namespace _system {

constexpr int kSTDIN_FD = 0;
constexpr int kSTDOUT_FD = 1;
constexpr int kSTDERR_FD = 2;

/* Write a string to a file identified by the file descriptor
 * Return: the number of the written characters or -errno
 */
[[nodiscard]] inline ssize_t write(const int fd, std::string_view str);

// Convenience function to write to STDOUT
[[nodiscard]] inline ssize_t write(std::string_view str) {
  return write(kSTDOUT_FD, str);
}

// Prohibit implicit length computation in write functions
[[nodiscard]] inline ssize_t write(const int fd, const char *str) = delete;
[[nodiscard]] inline ssize_t write(const char *str) = delete;

// Exit the process
inline void exit(const int status);

} // namespace _system

#if __STDC_HOSTED__ == 0
// -----Program entry point/exit logic for the freestanding mode------
extern "C" {
void _start() { _system::exit(main()); }
}
// --------------Freestanding system call implementations-------------
#if defined(__x86_64__)
#include "system_linux_x86_64.hpp"
#else
#error Freestanding implementation is not provided for the current system
#endif

#else
// -----Fallback for a hosted build in generic POSIX environment-------
#include "system_posix.hpp"

#endif
