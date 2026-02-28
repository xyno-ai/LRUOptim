cmake_minimum_required(VERSION 3.14)
project(LRUOptim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Google Test via FetchContent ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Test executable ---
add_executable(lru_tests tests/TestLRUCache.cpp)
target_include_directories(lru_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lru_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lru_tests)
