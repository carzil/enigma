include(GenerateExportHeader)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp)

add_subdirectory(common)
add_subdirectory(codecs)

add_library(enigma ${SOURCES})

generate_export_header(enigma
    EXPORT_MACRO_NAME ENIGMA_API
)

target_include_directories(enigma INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS enigma DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
