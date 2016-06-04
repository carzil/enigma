include_directories(
    ${ENIGMA_INCLUDE_LIBS}
)

find_package(Threads)

add_executable(runner runner.cpp)
target_link_libraries(runner enigma ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(runner INTERFACE enigma)
