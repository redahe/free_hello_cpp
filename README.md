# Hello World on Freestanding C++

   A C++ Hello World implementation that compiles and runs in the freestanding
mode on Linux x86_64 and falls back to rely on the standard library on
other POSIX systems. Includes inline asm for necessary syscall invocations and
a CMake script focused on optimizing the binary size.

# Motivation:
   Tiny programs like "Hello World", should not be above a kilobyte in the
compiled form by default. On x86_64 Linux the resulting binary is only 656
bytes. 

   The project could be used as a template for minimalist/hobbyist C++ projects 
that should support a fallback portability out of the box. 
(Think about small utilities that could fit a standard memory page of size 4Kb, 
projects with no dynamic memory allocation/custom memory management or
demoscene).


# Build instructions:
```
    mkdir build 
    cd build
    cmake ..
    make
```
