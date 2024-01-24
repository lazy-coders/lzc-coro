cmake_minimum_required(VERSION 3.25)
project(lzc-coro VERSION 0.1.0
                 DESCRIPTION "A simple header-only C++ library to use coroutines"
                 HOMEPAGE_URL "https://github.com/lazy-coders/lzc-coro"
                 LANGUAGES CXX)

include(cmake/CPM.cmake)

add_library(lzc-coro)
target_compile_features(lzc-coro PUBLIC cxx_std_23)
target_sources(lzc-coro PRIVATE src/lzcoders/default_execution_context.cpp)
target_include_directories(lzc-coro PUBLIC include)
target_compile_options(lzc-coro PUBLIC "-fcoroutines")
target_include_directories(
    lzc-coro
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)



include(GNUInstallDirs)
install(TARGETS lzc-coro
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#include(CMakePackageConfigHelpers)
#write_basic_package_version_file("lzc-coroConfigVersion.cmake"
#                                 VERSION ${PROJECT_VERSION}
#                                 COMPATIBILITY SameMajorVersion)
#configure_package_config_file(
#    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
#    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    INSTALL_DESTINATION
#    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_COVERAGE "If ENABLE_TESTS is ON then coverage could be produced" OFF)
if(ENABLE_TESTS)
    enable_testing()
    if(ENABLE_COVERAGE)
        include(cmake/coverage.cmake)
        add_coverage_target("*/tests/*")
    endif()
    add_subdirectory(tests)
endif()