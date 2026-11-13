cmake_minimum_required(VERSION 3.14)
project(mp2_lab3_translator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Для Windows, чтобы не было конфликтов CRT
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests
    test_main.cpp
    test_vector.cpp
    test_stack.cpp
    test_queue.cpp
    test_translator.cpp
)

target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(run_tests GTest::gtest)

include(GoogleTest)
gtest_discover_tests(run_tests)