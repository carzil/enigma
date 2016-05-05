#include <iostream>
#include <fstream>
#include <vector>
#include <gtest/gtest.h>
#include "huffman/huffman.h"
#include "tests_common/tests_common.h"


TEST(HuffmanCodecTest, SimpleEncodeDecodeTest) {
    Codecs::HuffmanCodec codec;
    test_ed_string(codec, "aaaaab");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
