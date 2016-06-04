cmake_minimum_required(VERSION 3.0)
project(enigma)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ENIGMA_BUILD_TESTS "Build tests for enigma" OFF)

set(ENIGMA_SANITIZER "" CACHE STRING "")

function(add_flag flag)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
endfunction()

add_flag("-std=c++14")
add_flag("-Wall -Wextra")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_flag("-Ofast")
    add_flag("-fno-omit-frame-pointer")
    add_flag("-g")
    if (ENIGMA_SANITIZER) 
        add_flag("-fsanitize=${ENIGMA_SANITIZER}")
    endif()

    add_definitions(-DDEBUG)
else()
    add_flag("-Ofast -march=native")
endif()

if (ENIGMA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    # because Enigma have some private API, hidden in shared library,
    # we need use static linking
    set(BUILD_SHARED_LIBS OFF)
endif()

add_subdirectory(enigma)
add_subdirectory(tools)

