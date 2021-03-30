add_executable(
  test_impl_ptr
  allocator.hpp
  impl.cpp
  impl_always_inplace.cpp
  impl_copied.cpp
  impl_inplace.cpp
  impl_poly.cpp
  impl_shared.cpp
  impl_unique.cpp
  main.cpp
  test.hpp)

target_link_libraries(test_impl_ptr PRIVATE gtest_main ${PROJECT_NAME}::${PROJECT_NAME})

add_test(NAME test_impl_ptr_google_test COMMAND $<TARGET_FILE:test_impl_ptr>)
