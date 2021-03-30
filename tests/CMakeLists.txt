cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

if(ENABLE_UNIT_TESTS)
  # fatch googletest since 3.11
  include(FetchContent)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://gitee.com/ghowoght/googletest.git
    GIT_TAG release-1.10.0)

  FetchContent_GetProperties(googletest)

  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    # prevent googletest from overriding our comiler/linker options when
    # building with VS
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
    # prevent googletest from using pthreads
    set(gtest_disable_pthreads
        ON
        CACHE BOOL "" FORCE)

    # add targets
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

    # Silence std::tr1 warning on MSVC
    if(MSVC)
      foreach(_tgt gtest gtest_main gmock gmock_main)
        target_compile_definitions(
          ${_tgt} PRIVATE "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
      endforeach()
    endif()
  endif()

  # all-in-one test module
  add_subdirectory(all_in_one)
endif()
