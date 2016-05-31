#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

#include "enigma/common/dictionary.h"

TEST(DictionaryTest, AddTest) {
    Codecs::Dictionary dict;
    int a = dict.AddNode(0, 'a');
    int b = dict.AddNode(0, 'b');
    int c = dict.AddNode(0, 'c');
    ASSERT_EQ(dict.NextNode(0, 'a'), a);
    ASSERT_EQ(dict.NextNode(0, 'b'), b);
    ASSERT_EQ(dict.NextNode(0, 'c'), c);
}

TEST(DictionaryTest, AddChainTest) {
    Codecs::Dictionary dict;
    int a = dict.AddNode(0, 'a');
    int b = dict.AddNode(a, 'b');
    int c = dict.AddNode(b, 'c');
    ASSERT_EQ(dict.NextNode(0, 'a'), a);
    ASSERT_EQ(dict.NextNode(a, 'b'), b);
    ASSERT_EQ(dict.NextNode(b, 'c'), c);
}

TEST(DictionaryTest, AddRandomStringTest) {
    Codecs::Dictionary dict;
    std::string str;
    for (size_t i = 0; i < 100; i++) {
        str.push_back(rand() % 256);
    }
    int ptr = dict.GetRoot();
    for (unsigned char c : str) {
        ptr = dict.AddNode(ptr, c);
    }
    ptr = dict.GetRoot();
    for (unsigned char c : str) {
        ptr = dict.NextNode(ptr, c);
        ASSERT_GT(ptr, 1);
    }
}

TEST(Dictionary, AddSameString) {
    Codecs::Dictionary dict;
    std::vector<std::string> v = { "abcd", "abcd", "abcd", "abcd", "abcd" };
    for (size_t i = 0; i < v.size(); i++) {
        int ptr = dict.GetRoot();
        for (size_t j = 0; j < v[i].size(); j++) {
            ptr = dict.AddNode(ptr, v[i][j]);
        }
        ASSERT_EQ(dict.RestoreString(ptr), v[i]);
    }
}

TEST(DictionaryTest, RestoreStringTest) {
    Codecs::Dictionary dict;
    std::string str;
    for (size_t i = 0; i < 100; i++) {
        str.push_back(rand() % 256);
    }
    int ptr = dict.GetRoot();
    for (unsigned char c : str) {
        ptr = dict.AddNode(ptr, c);
    }
    std::string s = dict.RestoreString(ptr);
    ASSERT_EQ(s, str);
}

TEST(DictionaryTest, AddRandomStringsTest) {
    Codecs::Dictionary dict;
    std::vector<std::string> v(5);
    for (size_t i = 0; i < v.size(); i++) {
        int ptr = dict.GetRoot();
        for (size_t j = 0; j < 2; j++) {
            v[i].push_back('0' + rand() % 10);
            ptr = dict.AddNode(ptr, v[i][j]);
        }
    }
    for (size_t i = 0; i < v.size(); i++) {
        int ptr = dict.GetRoot();
        for (size_t j = 0; j < v[i].size(); j++) {
            ptr = dict.NextNode(ptr, v[i][j]);
            ASSERT_GT(ptr, 1);
        }
        std::string s = dict.RestoreString(ptr);
        ASSERT_EQ(s, v[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}