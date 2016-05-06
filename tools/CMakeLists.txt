include_directories(
    ${ENIGMA_INCLUDE_LIBS}
)

add_executable(runner runner.cpp)
target_link_libraries(runner enigma)
target_include_directories(runner INTERFACE enigma)
