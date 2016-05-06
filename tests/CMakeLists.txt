find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_LIBS}
)

enable_testing()

function(enigma_test)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ETEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    message("${ETEST_NAME} ${ETEST_SOURCES}")
    add_executable(${ETEST_NAME} ${ETEST_SOURCES})
    target_link_libraries(${ETEST_NAME} enigma ${GTEST_LIBRARIES})
endfunction()

enigma_test(NAME huffman_tests SOURCES huffman_tests.cpp)
