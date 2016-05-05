#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

#include "huffman/huffman.h"
#include "tests_common/tests_common.h"


TEST(HuffmanCodecTest, SimpleEncodeDecodeTest) {
    Codecs::HuffmanCodec codec;
    test_ed_string(codec, "abcde");
}

TEST(HuffmanCodecTest, ConsecutiveEncodeDecodeTest) {
    Codecs::HuffmanCodec codec;
    test_ed_string(codec, "abcde");
    test_ed_string(codec, "basda");
    test_ed_string(codec, "qwert");
    test_ed_string(codec, "acula");
}

TEST(HuffmanCodecTest, BigCodewordTest) {
    Codecs::HuffmanCodec codec;
    std::string test_string;
    test_string.reserve(1 << 15);
    for (int i = 0; i <= 14; i++) {
        for (int j = 0; j < (1 << i); j++) {
            test_string.push_back('0' + i);
        }
    }
    test_ed_string(codec, test_string);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
