cmake_minimum_required(VERSION 3.0)
project(enigma)

enable_testing()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Ofast -march=native")

add_subdirectory(enigma)
add_subdirectory(tools)
