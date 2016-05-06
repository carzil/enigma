find_package(GTest REQUIRED)
include(GenerateExportHeader)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GTEST_INCLUDE_LIBS}
)


set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_subdirectory(common)
add_subdirectory(huffman)

add_library(enigma ${SOURCES})
generate_export_header(enigma
    EXPORT_MACRO_NAME ENIGMA_API
)

target_include_directories(enigma INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
