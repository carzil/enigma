cmake_minimum_required(VERSION 3.0)

project(codecs)

enable_testing()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O0 -Wall -Wextra -fsanitize=address")
endif()

add_subdirectory(external)
add_subdirectory(library)
add_subdirectory(tools)
