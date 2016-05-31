#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <cstring>

#include "enigma/huffman/huffman.h"
#include "enigma/utils.h"
#include "enigma/colors.h"


namespace Codecs {

void HuffmanCodec::GenerateCodes() {
    vector<bool> bits;
    auto codes = tree->GenerateCodes();

    for (auto p : codes) {
        Codeword* cw = new Codeword(p.second);
        prefix_table->AddCodeword(cw, p.first);
        char_table[p.first] = cw;
    }

    tree->Inorder();
}


HuffmanCodec::HuffmanCodec() {
    tree = new HuffmanTree();

    for (size_t j = 0; j < 256 + 2; j++) {
        char_table[j] = nullptr;
    }

    prefix_table = new PrefixTable();

    memset(frequencies, 0, (256 + 2) * sizeof(size_t));
}

HuffmanCodec::~HuffmanCodec() {
    if (tree != nullptr) {
        delete tree;
    }
    if (prefix_table != nullptr) {
        delete prefix_table;
    }
    for (size_t j = 0; j < 256 + 2; j++) {
        if (char_table[j] != nullptr) {
            delete char_table[j];
        }
    }
}

void HuffmanCodec::Learn(const vector<string>& vec) {
    for (const string& str : vec) {
        Learn(str);
    }
}

void HuffmanCodec::Learn(const string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        frequencies[static_cast<uint8_t>(str[i])]++;
    }
}

void HuffmanCodec::EndLearning() {
    for (size_t i = 0; i < 256 + 2; i++) {
        if (frequencies[i] > 0 || i == 256) {
            tree->PushValue(i, frequencies[i]);
        }
    }
    tree->Build();
    GenerateCodes();
}

size_t HuffmanCodec::encode(const string_view& raw, char* encoded) {
    uint8_t bitContainer = 0;
    size_t bitOffset = 0;
    char* start = encoded;
    for (size_t i = 0; i < raw.size() + 1; i++) {
        unsigned int ch;
        if (i != raw.size()) {
            ch = static_cast<uint8_t>(raw[i]);
        } else {
            ch = 256;
        }
        Codeword* cw = char_table[ch];
        // std::cout << "merging codeword " << ch << " = " << *tree->char_table[0][ch] << ", offset is " << bitOffset << std::endl;
        bitContainer |= cw->packedBits[bitOffset][0];
        if (cw->size[bitOffset] > 1) {
            memcpy(encoded, cw->packedBits[bitOffset], cw->size[bitOffset] - 1);
            encoded += cw->size[bitOffset] - 1;
            // encoded.push_back(bitContainer);
            // for (size_t j = 1; j < cw->size[bitOffset] - 1; j++) {
            //     encoded.push_back(cw->packedBits[bitOffset][j]);
            // }
            bitContainer = cw->packedBits[bitOffset][cw->size[bitOffset] - 1];
        }
        bitOffset = cw->lastBitsCount[bitOffset];
    }
    if (bitOffset > 0) {
        *encoded = bitContainer;
        encoded++;
    }

    return encoded - start;

    // for (char c : encoded) {
    //     int bitPos = 7;
    //     while (bitPos >= 0) {
    //         bool bit = c & (1 << bitPos);
    //         std::cout << bit;
    //         bitPos--;
    //     }
    // }
    // std::cout << std::endl;
    // return string_view(encoded, ptr - encoded);
}

size_t HuffmanCodec::decode(const string_view& raw, char* result) {
    // result.reserve(2 * raw.size());
    // int bitPos = 0;
    // size_t pos = 1;
    // uint8_t bitContainerLeft = raw[0];
    // uint8_t bitContainerRight = 0;
    // if (raw.size() > 1) {
    //     bitContainerRight = raw[1];
    // }
    // uint8_t bits = 0;
    // bool decoding = true;
    // PrefixTable* table = prefix_table;
    // while (decoding) {
    //     bits = (bitContainerLeft << bitPos) | (bitContainerRight >> (8 - bitPos));
    //     // print_char_bits(bits); std::cout << " "; print_char_bits(bitContainerLeft); std::cout << " "; print_char_bits(bitContainerRight); std::cout << " " << bitPos << std::endl;
    //     PrefixTable::PrefixTableEntry* entry = table->entries[bits];
    //     if (entry->nextTable == nullptr) {
    //         int symbol = entry->symbol;
    //         if (symbol != 256) {
    //             result.push_back(symbol);
    //             bitPos += entry->length;
    //         } else {
    //             decoding = false;
    //             break;
    //         }
    //         table = prefix_table;
    //     } else {
    //         table = entry->nextTable;
    //         bitPos += 8;
    //     }
    //     if (bitPos >= 8) {
    //         bitPos -= 8;
    //         bitContainerLeft = raw[pos];
    //         if (pos + 1 < raw.size()) {
    //             bitContainerRight = raw[pos + 1];
    //         } else {
    //             bitContainerRight = 0;
    //         }
    //         pos++;
    //     }
    // }
    // std::cout << std::endl;
    return 0;
}

string HuffmanCodec::save() const {
    return string();
}

void HuffmanCodec::load(const string_view&) {
}

size_t HuffmanCodec::sample_size(size_t total) const {
    return total / 10;
}

void HuffmanCodec::reset() {
    if (tree != nullptr) {
        tree->Reset();
    }

    for (size_t j = 0; j < 256 + 2; j++) {
        if (char_table[j] != nullptr) {
            delete char_table[j];
            char_table[j] = nullptr;
        }
    }

    if (prefix_table != nullptr) {
        delete prefix_table;
        prefix_table = new PrefixTable();
    }
}

}
