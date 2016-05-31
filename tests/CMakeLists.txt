find_package(GTest REQUIRED)

include_directories(
    ${GTEST_INCLUDE_LIBS}
)

function(enigma_test)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ETEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    message("${ETEST_NAME} ${ETEST_SOURCES}")
    add_executable(${ETEST_NAME} ${ETEST_SOURCES})
    target_link_libraries(${ETEST_NAME} enigma ${GTEST_LIBRARIES})
    add_test(NAME "enigma_${ETEST_NAME}" COMMAND ${ETEST_NAME})
endfunction()

enigma_test(NAME huffman_tests SOURCES huffman_tests.cpp)
enigma_test(NAME dictionary_tests SOURCES dictionary_tests.cpp)
