// #include <iostream>
// #include <fstream>
// #include <gtest/gtest.h>
// #include <vector>

// #include "enigma/huffman/huffman.h"
// #include "enigma/huffman/prefix_table.h"
// #include "tests_common.h"


// TEST(HuffmanCodecTest, SimpleEncodeDecodeTest) {
//     Codecs::HuffmanCodec codec;
//     test_ed_string(codec, "abcde");
// }

// TEST(HuffmanCodecTest, ConsecutiveEncodeDecodeTest) {
//     Codecs::HuffmanCodec codec;
//     test_ed_string(codec, "abcde");
//     test_ed_string(codec, "basda");
//     test_ed_string(codec, "qwert");
//     test_ed_string(codec, "acula");
// }

// TEST(HuffmanCodecTest, BigCodewordTest) {
//     Codecs::HuffmanCodec codec;
//     std::string test_string;
//     test_string.reserve(1 << 15);
//     for (int i = 0; i <= 14; i++) {
//         for (int j = 0; j < (1 << i); j++) {
//             test_string.push_back('0' + i);
//         }
//     }
//     test_ed_string(codec, test_string);
// }

// TEST(CodewordtTest, PackSimple) {
//     std::vector<bool> bits = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
//     Codecs::Codeword cw(bits, 0);
//     ASSERT_EQ(cw.size, 2);
//     ASSERT_EQ(cw.packedBits[0], 255);
//     ASSERT_EQ(cw.packedBits[1], 0);
// }

// TEST(CodewordTest, PackIncomplete) {
//     std::vector<bool> bits = {1, 1};
//     Codecs::Codeword cw(bits, 0);
//     ASSERT_EQ(cw.size, 1);
//     ASSERT_EQ(cw.packedBits[0], 192);
//     ASSERT_EQ(cw.lastBitsCount, 2);
// }

// TEST(CodewordTest, PackExactOneByte) {
//     std::vector<bool> bits = {1, 1, 1, 1, 1, 1, 1, 1};
//     Codecs::Codeword cw(bits, 0);
//     ASSERT_EQ(cw.size, 1);
//     ASSERT_EQ(cw.packedBits[0], 255);
// }

// TEST(CodewordTest, PackAllBytes) {
//     for (int i = 0; i <= (1 << 7); i++) {
//         std::vector<bool> bits;
//         int bitPos = 7;
//         while (bitPos >= 0) {
//             bits.push_back(i & (1 << bitPos));
//             bitPos--;
//         }
//         Codecs::Codeword cw(bits, 0);
//         ASSERT_EQ(cw.packedBits[0], i);
//     }
// }

// TEST(CodewordTest, PackWithOffset) {
//     for (int i = 1; i < 8; i++) {
//         Codecs::Codeword cw({1}, i);
//         ASSERT_EQ(cw.size, 1);
//         ASSERT_EQ(cw.packedBits[0], 128 >> i);
//     }
//     Codecs::Codeword cw({1}, 8);
//     ASSERT_EQ(cw.size, 2);
//     ASSERT_EQ(cw.packedBits[0], 0);
//     ASSERT_EQ(cw.packedBits[1], 128);
// }

// TEST(PrefixTableTest, SimpleTest) {
//     Codecs::Codeword cw1({1, 1}, 0);
//     Codecs::Codeword cw2({1, 0}, 0);
//     Codecs::PrefixTable table;
//     table.AddCodeword(&cw1, 1);
//     table.AddCodeword(&cw2, 2);
//     ASSERT_EQ(table.entries[192]->symbol, 1);

//     // all masks 11xxxxxx
//     for (int i = 0; i <= (1 << 5); i++) {
//         ASSERT_EQ(table.entries[192 | i]->symbol, 1);
//     }
//     // all masks 10xxxxxx
//     for (int i = 0; i <= (1 << 5); i++) {
//         ASSERT_EQ(table.entries[128 | i]->symbol, 2);
//     }

//     ASSERT_EQ(table.entries[128]->symbol, 2);
// }

// TEST(PrefixTableTest, OneChainTest) {
//     std::vector<bool> bits = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     Codecs::Codeword cw(bits, 0);
//     Codecs::PrefixTable table;
//     table.AddCodeword(&cw, 1);
//     ASSERT_NE(table.entries[255], nullptr);
//     ASSERT_NE(table.entries[255]->nextTable, nullptr);

//     for (int i = 0; i <= (1 << 6); i++) {
//         ASSERT_EQ(table.entries[255]->nextTable->entries[128 | i]->symbol, 1);
//     }
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
