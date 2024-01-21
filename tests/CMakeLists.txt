#CPMAddPackage(
#  NAME googletest
#  GITHUB_REPOSITORY google/googletest
#  GIT_TAG release-1.12.1
#  VERSION 1.12.1
#  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
#)

#add_test(NAME lzc-coro-test )
add_executable(lzc-coro-test main.cpp)
target_link_libraries(lzc-coro-test  PRIVATE lzc-coro)
#target_include_directories(coro PUBLIC include)
