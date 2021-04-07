cmake_minimum_required(VERSION 3.11)

enable_language(CXX)

# Don't warn about [[noreturn]] for older GCC/Clang that only know
# [[gnu::noreturn]].
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Wno-error=unknown-attributes"
                        NO_ERROR_UNKNOWN_ATTRIBUTES)
check_cxx_compiler_flag("-Wno-error=attributes" NO_ERROR_ATTRIBUTES)
if(NO_ERROR_UNKNOWN_ATTRIBUTES)
  set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes ${CMAKE_CXX_FLAGS}")
elseif(NO_ERROR_ATTRIBUTES)
  set(CMAKE_CXX_FLAGS "-Wno-attributes ${CMAKE_CXX_FLAGS}")
endif()

if(UNIX
   AND ((CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
            AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
   AND CMAKE_GENERATOR STREQUAL "Ninja")
  # These compilers generate coloured output, but by default only when their
  # output channel is a terminal (TTY/PTY). Ninja however captures all output in
  # a pipe (per-subprocess), disabling coloured compiler diagnostics. We
  # forcefully enable it because Ninja, since 1.0.0 (ninja-build/ninja#198)
  # takes care to strip VT100 CSI control sequences from the output if Ninja
  # itself is writing to a pipe instead of a terminal. As a result this should
  # give us the best of both worlds: coloured output when we're running in a
  # terminal, plain output for editors, IDEs, etc.
  set(CMAKE_CXX_FLAGS
      "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}"
      CACHE STRING "" FORCE)
endif()

# Prepend CMake's auto-detected required C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} ${CMAKE_CXX_FLAGS}")

project(
  pimpl
  LANGUAGES CXX
  VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

# GNU install standard
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(INSTALL_INCLUDEDIR
    ${CMAKE_INSTALL_INCLUDEDIR}
    CACHE PATH "Installation directory for header files")

set(DEF_INSTALL_CMAKEDIR share/${PROJECT_NAME})
set(INSTALL_CMAKEDIR
    ${DEF_INSTALL_CMAKEDIR}
    CACHE PATH "Installation directory for CMake files")

# Report to user
foreach(p INCLUDE CMAKE)
  file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
  message(STATUS "Show installing ${p} components to native path :${_path}")
  unset(_path)
endforeach()

add_definitions(-D_DISABLE_EXTENDED_ALIGNED_STORAGE
                -D_ENABLE_EXTENDED_ALIGNED_STORAGE)

add_subdirectory(include)

if(FALSE)
  # we will use the network to fetch Google Test sources make it possible to
  # disable unit tests when not on network
  option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
  message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

  # enable testing and define tests
  enable_testing()

  # add memcheck test action
  include(CTest)

  add_subdirectory(tests)
endif()
