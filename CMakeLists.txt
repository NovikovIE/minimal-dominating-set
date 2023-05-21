cmake_minimum_required(VERSION 3.23)
project(minimal_dominating_set)

set(CMAKE_CXX_STANDARD 20)

# FetchContent
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

message(STATUS "Fetching GoogleTest")
FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest/"
        GIT_TAG "e2239ee6043f73722e7aa812a459f54a28552929"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(googletest)

# Creates the executable main_exe
add_executable(
        minimal_dominating_set
        blossom.cpp
        graph.cpp
        min_edge_cover.cpp
        test.cpp
        minimal_dominating_set.h)

# Links the libraries to the executable
target_link_libraries(
        minimal_dominating_set
        gtest_main
        gmock_main
)

include(GoogleTest)
gtest_discover_tests(minimal_dominating_set)