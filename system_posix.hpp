/*
 *  Implementation for system calls that relies on stdlib in POSIX environment
 */

#pragma once

#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

#include <cstdlib>  // For std::exit
#include <errno.h>  // For errno
#include <unistd.h> // For ::write

namespace _system {

[[nodiscard]] inline ssize_t write(const int fd, std::string_view str) {
  ssize_t ret = ::write(fd, str.data(), str.size());
  if (ret == -1) {
    return -errno;
  }
  return ret;
}

void inline exit(const int status) { std::exit(status); }

} // namespace _system
