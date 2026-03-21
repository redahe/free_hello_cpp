# "Hello, World!" on freestanding C++

   A "Hello, World!" implementation on freestanding C++ to run on Linux x86_64,
supports compilation for other POSIX systems by falling back to the hosted
implementation.
Includes inline asm for necessary syscall invocations and a CMake script
focused on optimizing the binary size (*it by default gives up many
security protections*, which can be tweaked in the settings).


## Motivation

   Tiny single-user programs like "Hello, World!" should not be larger than a
kilobyte in the compiled form by default. This project achieves this goal -
on the target platform, with all security features disabled the program payload
in the resulting executable is only 49 bytes of the machine code identical
to the one that would be produced by a handcrafted assembly code,
the ELF binary size is 512 bytes, 225 bytes after sstrip and could be reduced
further by messing with the ELF program headers.

   This program and the build scripts could be used as an outline for
minimalist/hobbyist freestanding C++ projects with some portability in mind.
Think about small utilities that could fit a standard memory page of size 4Kb,
projects with no dynamic memory allocation or projects with custom memory
management, demoscene.


# The default security settings

By default the program compiles with the following compiler and linker
flags, which disable the standard security features in the compiled
code and the executable in order to decrease the result binary size:

  -fno-stack-protector
  -fcf-protection=none
  -Wl,-z,norelro
  -Wl,-z,noseparate-code
  -Wl,-z,common-page-size=16

Remove/change these settings if the security aspect is important,
(the stack protector relies on functions/variables defined in libc
which need to be ported or re-implemented in the freestanding mode).

## Build instructions

Requirements: make, cmake, c++17 or above.  
Optional: strip, sstrip (for minimizing the binary size).


Run `make` from the project directory to build the project.  
Run `make sstrip` to run sstrip for aggressive ELF size reduction.  
Run `make help` to see all supported commands.
