project(codecs)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O2 -Wall -Wextra -Werror")
endif()

add_subdirectory(external)
add_subdirectory(library)
add_subdirectory(tools)
