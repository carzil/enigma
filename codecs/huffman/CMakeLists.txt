target_lib(
    SOURCES huffman.h huffman.cpp
    LINK_DEPS codecs-common
)

add_subdirectory(test)
