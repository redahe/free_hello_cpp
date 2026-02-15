cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Run cmake from a build directory!")
endif()


# =============== Project specifics ================
project(hello)

set(SOURCES hello.cpp)
add_executable(hello ${SOURCES})

# --- Minimal compiler version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ---- Target systems to build in freestanding mode
set(SUPPORTED_FREESTANDING_SYSTEMS Linux)
set(SUPPORTED_FREESTANDING_ARCHS x86_64)
# ==================================================


# =============== Diagnostic messages ==============
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
# ==================================================


# =============== Setting appropriate flags =========
# ---- Set common flags to optimize for binary size
target_compile_options(hello PRIVATE  
  -Os
  -fno-exceptions
  -fno-asynchronous-unwind-tables
  -fno-unwind-tables
  -fno-ident
)
target_link_options(hello PRIVATE  
   -Wl,--build-id=none
   -Wl,-R,.eh_frame
   -Wl,--gc-sections 
   -Wl,-z,max-page-size=0x1000     # Not > than standard memory page size 4Kb
)

# --------Define flags that trade security for binary size------
set(OPTIMIZE_SIZE_OVER_SECURITY_COMPILE_FLAGS
  -fno-stack-protector             # No stack overflow protection
)

set(OPTIMIZE_SIZE_OVER_SECURITY_LINK_FLAGS
   -Wl,-z,norelro                  # no RELRO to save header space
   -Wl,-z,noseparate-code          # no separate sections for code/data
   -Wl,-z,common-page-size=16      # but align sections to 16, to support SIMD
)

# ------ Freestanding build if applicable---------------------
if(CMAKE_SYSTEM_NAME IN_LIST SUPPORTED_FREESTANDING_SYSTEMS AND 
    CMAKE_SYSTEM_PROCESSOR IN_LIST SUPPORTED_FREESTANDING_ARCHS)

  set(FREESTANDING_COMPILE_FLAGS
      -ffreestanding
      -fno-stack-protector    # stack protector is not implemented yet
  )

  set(FREESTANDING_LINK_FLAGS
     -nostdlib
     -nostartfiles
     -static
     -Wl,-z,norelro    # save space, RELRO is not very useful with -static
  )
  message(STATUS 
    "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR} - buidling FREESTANDING")

  target_compile_options(hello PRIVATE  ${FREESTANDING_COMPILE_FLAGS})
  target_link_options(hello PRIVATE  ${FREESTANDING_LINK_FLAGS})

  # Trade security for binary size in freestanding mode by default
  target_compile_options(hello PRIVATE  
    ${OPTIMIZE_SIZE_OVER_SECURITY_COMPILE_FLAGS})
  target_link_options(hello PRIVATE  
    ${OPTIMIZE_SIZE_OVER_SECURITY_LINK_FLAGS})
else()
  message(STATUS 
    "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR} - building HOSTED")
endif()

# ------ Clean  unused code  -----------------------------------
if(FORCE_NO_LTO)
  set(lto_supported 0)
  set(error FORCED_NO_LTO)
else()
  include(CheckIPOSupported)
  check_ipo_supported(RESULT lto_supported OUTPUT error)
endif()

if(lto_supported)
  message(STATUS "Using LTO")
    set_target_properties(hello PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS 
      "LTO not supported: ${error}, using -f*-sections and -Wl,--gc-sections")
    target_compile_options(hello PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(hello PRIVATE "-Wl,--gc-sections")
endif()
# ==================================================


# ============= Post process the binary============
# Strip unnecessary sections from the binary
add_custom_command(TARGET hello POST_BUILD
  COMMAND ${CMAKE_STRIP} 
      --strip-all 
      --remove-section .comment 
      --remove-section .note 
      --remove-section .note.gnu.property 
      $<TARGET_FILE:hello>
  COMMENT "Striping the binary"
)
# ==================================================


# ============= Print the results ==================
add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "--------------------------------"
    COMMAND ls -lh $<TARGET_FILE:hello>
    COMMAND ${CMAKE_COMMAND} -E echo "--------------------------------"
    COMMENT "Final binary size:"
)
# ==================================================
