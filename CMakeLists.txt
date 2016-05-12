cmake_minimum_required(VERSION 3.0)
project(enigma)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(enigma_build_tests "Build tests for enigma" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Ofast -march=native")

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_definitions(-DDEBUG)
endif()

if (enigma_build_tests)
    enable_testing()
    add_subdirectory(tests)
    # because Enigma have some private API, hidden in shared library,
    # we need use static linking
    set(BUILD_SHARED_LIBS OFF)
endif()

add_subdirectory(enigma)
add_subdirectory(tools)

