find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GTEST_INCLUDE_LIBS}
)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Ofast -Wall -Wextra -fsanitize=address")
endif()

add_subdirectory(common)
add_subdirectory(huffman)
add_subdirectory(tests_common)
add_subdirectory(tools)
