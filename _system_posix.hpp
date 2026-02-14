/*
 *  Implementation for system calls that relies on stdlib in POSIX environment
 */

#pragma once

#include <cstddef>     // For size_t
#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

#include <cstdlib>
#include <errno.h>  // For errno
#include <unistd.h> // For POSIX 'write' implementation from the standard lib

namespace _system {

[[nodiscard]] ssize_t write(const int fd, std::string_view msg) {
  ssize_t ret = ::write(fd, msg.data(), msg.size());
  if (ret == -1) {
    return -errno;
  }
  return ret;
}

void exit(const int status) { std::exit(status); }

} // namespace _system
