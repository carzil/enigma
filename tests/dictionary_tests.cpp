#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

#include "enigma/common/dictionary.h"

TEST(DictionaryTest, AddTest) {
    Enigma::Dictionary dict;
    int a = dict.AddNode(dict.GetRoot(), 'a');
    int b = dict.AddNode(dict.GetRoot(), 'b');
    int c = dict.AddNode(dict.GetRoot(), 'c');
    ASSERT_EQ(dict.NextNode(dict.GetRoot(), 'a'), a);
    ASSERT_EQ(dict.NextNode(dict.GetRoot(), 'b'), b);
    ASSERT_EQ(dict.NextNode(dict.GetRoot(), 'c'), c);
}

TEST(DictionaryTest, AddChainTest) {
    Enigma::Dictionary dict;
    int a = dict.AddNode(dict.GetRoot(), 'a');
    int b = dict.AddNode(a, 'b');
    int c = dict.AddNode(b, 'c');
    ASSERT_EQ(dict.NextNode(dict.GetRoot(), 'a'), a);
    ASSERT_EQ(dict.NextNode(a, 'b'), b);
    ASSERT_EQ(dict.NextNode(b, 'c'), c);
}

TEST(DictionaryTest, AddRandomStringTest) {
    Enigma::Dictionary dict;
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
    Enigma::Dictionary dict;
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
    Enigma::Dictionary dict;
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
    Enigma::Dictionary dict;
    std::vector<std::string> v(5);
    for (size_t i = 0; i < v.size(); i++) {
        int ptr = dict.GetRoot();
        for (size_t j = 0; j < 20; j++) {
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

// TEST(DictionaryTest, LoadSaveSimpleTest) {
//     Enigma::Dictionary dict;
//     int a = dict.AddNode(dict.GetRoot(), 'a');
//     int b = dict.AddNode(dict.GetRoot(), 'b');
//     int c = dict.AddNode(dict.GetRoot(), 'c');
//     std::string saved = dict.Save();
//     Enigma::Dictionary dict2;
//     dict2.Load(saved);
//     ASSERT_EQ(dict2.NextNode(dict2.GetRoot(), 'a'), a);
//     ASSERT_EQ(dict2.NextNode(dict2.GetRoot(), 'b'), b);
//     ASSERT_EQ(dict2.NextNode(dict2.GetRoot(), 'c'), c);
// }

// TEST(DictionaryTest, LoadSaveRandomTest) {
//     Enigma::Dictionary dict;
//     std::vector<std::string> v(5);
//     for (size_t i = 0; i < v.size(); i++) {
//         int ptr = dict.GetRoot();
//         for (size_t j = 0; j < 20; j++) {
//             v[i].push_back('0' + rand() % 10);
//             ptr = dict.AddNode(ptr, v[i][j]);
//         }
//     }
//     std::string saved = dict.Save();
//     Enigma::Dictionary dict2;
//     dict2.Load(saved);
//     for (size_t i = 0; i < v.size(); i++) {
//         int ptr = dict2.GetRoot();
//         for (size_t j = 0; j < v[i].size(); j++) {
//             ptr = dict2.NextNode(ptr, v[i][j]);
//             ASSERT_GT(ptr, 1);
//         }
//         std::string s = dict2.RestoreString(ptr);
//         ASSERT_EQ(s, v[i]);
//     }
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}