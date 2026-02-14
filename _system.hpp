/*
 *  System call wrappers, and low level necessities.
 */

#pragma once

#include <string_view> // For std::string_view
#include <sys/types.h> // For ssize_t

int main();

namespace _system {

/* Write string_view to a file by a file descriptor.
 * Return: the number of the written characters or -errno
 */
[[nodiscard]] ssize_t write(const int fd, std::string_view msg);

/* Exit the process
 */
void exit(const int status);

} // namespace _system

#if __STDC_HOSTED__ == 0
// ------------Program entry point/exit in the freestanding mode------
extern "C" {
void _start() { _system::exit(main()); }
}
// --------------Freestanding system call implementations-------------
#if defined(__x86_64__)
#include "_system_linux_x86_64.hpp"
#else
#error Free standing implementation is not provided for the current system
#endif

#else
// ---------Fallback for hosted POSIX environment---------------------
#include "_system_posix.hpp"

#endif
