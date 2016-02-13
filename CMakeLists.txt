cmake_minimum_required(VERSION 3.0)
enable_testing()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/codecs" "${CMAKE_CURRENT_BINARY_DIR}/codecs")
add_subdirectory(codecs)
