# "Hello, World!" on freestanding C++

   A "Hello, World!" implementation on freestanding C++ to run on Linux x86_64,
supports compilation for other POSIX systems by falling back to the hosted
implementation.
Includes inline asm for necessary syscall invocations and a CMake script
focused on optimizing the binary size.


## Motivation
   Tiny programs like "Hello, World!" should not be larger than a kilobyte in
the compiled form by default. This project achieves this goal - the resulting
size of the executable on the target platform is only 656 bytes (373 bytes
after sstrip).

   This program and the build scripts could be used as an outline for
minimalist/hobbyist freestanding C++ projects with some portability in mind.
Think about small utilities that could fit a standard memory page of size 4Kb,
projects with no dynamic memory allocation or projects with custom memory
management, demoscene.



## Build instructions

Requirements: make, cmake, c++17 or above.  
Optional: strip, sstrip (for minimizing the binary size).


Run `make` from the project directory to build the project.  
Run `make sstrip` to run sstrip for aggressive ELF size reduction.  
Run `make help` to see all supported commands.
