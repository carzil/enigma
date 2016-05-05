target_lib(
    SOURCES huffman.h huffman.cpp
            tree.h tree.cpp
            prefix_table.h prefix_table.cpp
            codeword.h codeword.cpp
    LINK_DEPS codecs-common
)

add_subdirectory(test)
