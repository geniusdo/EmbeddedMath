cmake_minimum_required( VERSION 3.10)

project(EmbeddedMath)

option(BUILD_TEST "build test files" ON)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

if(BUILD_TEST)
    add_subdirectory(test)
endif(BUILD_TEST)