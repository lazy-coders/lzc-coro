cmake_minimum_required(VERSION 3.25)
project(lzc-coro VERSION 0.1.0
                 DESCRIPTION "A simple header-only C++ library to use coroutines"
                 HOMEPAGE "https://github.com/lzcoders/lzc-coro"
                 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 


add_library(lzc-coro INTERFACE)
target_include_directories(
    lzc-coro
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(lzc-coro INTERFACE cxx_std_23)


include(GNUInstallDirs)
install(TARGETS lzc-coro
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("lzc-coroConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
add_executable(coro main.cpp)
target_include_directories(coro PUBLIC include)