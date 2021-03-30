add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>)

target_compile_features(
  ${PROJECT_NAME}
  INTERFACE cxx_alias_templates
            cxx_auto_type
            cxx_constexpr
            cxx_decltype
            cxx_default_function_template_args
            cxx_defaulted_functions
            cxx_deleted_functions
            cxx_explicit_conversions
            cxx_final
            cxx_inheriting_constructors
            cxx_nonstatic_member_init
            cxx_nullptr
            cxx_override
            cxx_right_angle_brackets
            cxx_rvalue_references
            cxx_static_assert
            cxx_template_template_parameters
            cxx_variadic_templates)

# config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/pimplConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/pimplConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/pimplConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CMAKEDIR})

# install files
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${INSTALL_INCLUDEDIR}/pimpl
  COMPONENT dev
  PATTERN "CMakeLists.txt" EXCLUDE)

install(TARGETS pimpl EXPORT pimplTargets)

install(
  EXPORT pimplTargets
  NAMESPACE "${PROJECT_NAME}::"
  DESTINATION ${INSTALL_CMAKEDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pimplConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/pimplConfigVersion.cmake
        DESTINATION ${INSTALL_CMAKEDIR})
