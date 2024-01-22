project(lzc-coro-test)

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.12.1
  VERSION 1.12.1
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

add_executable(lzc-coro-test test_simple.cpp test_simulated_http.cpp)
target_link_libraries(lzc-coro-test  PRIVATE lzc-coro GTest::gtest_main GTest::gtest GTest::gmock)

add_test(lzc-coro-test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lzc-coro-test)
