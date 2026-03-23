
# "Hello, World!" on freestanding C++

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Description

   A "Hello, World!" implementation on freestanding C++ to run on Linux x86_64,
supports compilation for other POSIX systems by falling back to the hosted
implementation.
Includes inline asm for necessary syscall invocations and a CMake script
focused on optimizing the binary size (**note: achieving the minimal size
requires giving up many security features provided by the compiler**).


## Motivation

   Tiny single-user programs like "Hello, World!" should not be larger than a
kilobyte in the compiled form by default. This project achieves this goal -
on the target platform, with all security features disabled the program payload
in the resulting executable is only 48 bytes(33 bytes of the machine code
identical to the one that would be produced by a handcrafted assembly code and
15 bytes for the string literal). The ELF binary size is 512 bytes,
224 bytes after sstrip and could be reduced further by messing with the ELF
program headers.

   This program and the build scripts could be used as an outline for
minimalist/hobbyist freestanding C++ projects with some portability in mind.
Think about small utilities that could fit a standard memory page of size 4Kb,
projects with no dynamic memory allocation or projects with custom memory
management, demoscene.


## The default security settings

By default the program builds with the -static flag preventing any dynamic
linking, so RELRO is disabled. No other security features are disabled by
default, however when compiled for the freestanding environment
(on Linux x86_64) it uses a custom stack protector implementation,
which can be found in `system.cpp` (as the libc version not available).

When the project is configured to disable all security features
(by `make no_sec`) the following flags are used in order to
achieve the minimal binary size :

  -fno-stack-protector  
  -fcf-protection=none  
  -Wl,-z,norelro  
  -Wl,-z,noseparate-code  


## Build instructions

Requirements: make, cmake, c++17 or above.  
Optional: strip, sstrip (for minimizing the binary size).

Optionally run `make no_sec` to disable all 
security features to achieve the minimal binary size.
Run `make` from the project directory to build the project.  
Run `make sstrip` to run sstrip for aggressive ELF size reduction.  
Run `make help` to see all supported commands.
