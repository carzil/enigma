target_exe(
        NAME runner
        SOURCES runner.cpp
        LINK_DEPS codecs-huffman codecs-tests_common
)
