#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <cstring>

#include "huffman.h"


namespace Codecs {

void HuffmanCodec::GenerateCodes(HuffmanNode* node, vector<bool>& bits, size_t depth) {
    // std::cout << pos << std::endl;
    if (node == nullptr) {
        return;
    }

    bits.push_back(0);
    GenerateCodes(node->left, bits, depth + 1);
    bits.back() = 1;
    GenerateCodes(node->right, bits, depth + 1);
    bits.resize(bits.size() - 1);

    if (node->left == nullptr && node->right == nullptr) {
        for (size_t bitOffset = 0; bitOffset <= 8; bitOffset++) {
            Codeword* cw = new Codeword(bits, bitOffset);
            char_table[bitOffset][node->c] = cw;
        }
        // std::cout << "code for '" << (char)node->c << "' (";
        // std::cout << node->c << ") is " << *char_table[0][node->c] << ", depth = " << depth << std::endl;
        // std::cout << "======" << std::endl;
    }
}

void print_char_bits(char c) {
    int bitPos = 7;
    while (bitPos >= 0) {
        std::cout << bool(c & (1 << bitPos));
        bitPos--;
    }
}

void HuffmanCodec::FillSymbolTable(int symbol, size_t length, PrefixTable* table, int lastChunk, int pos) {
    if (pos == -1) {
        table->entries[lastChunk] = new PrefixTable::PrefixTableEntry();
        table->entries[lastChunk]->symbol = symbol;
        table->entries[lastChunk]->length = length;
        table->entries[lastChunk]->nextTable = nullptr;
        // print_char_bits(lastChunk); std::cout << " -> '" << (char)symbol << "' (" << symbol << "), execess = " << length << std::endl;
    } else {
        FillSymbolTable(symbol, length, table, lastChunk | (1 << pos), pos - 1);
        FillSymbolTable(symbol, length, table, lastChunk, pos - 1);
    }
}

void HuffmanCodec::GenerateCodes() {
    vector<bool> bits;
    GenerateCodes(tree->root, bits, 0);

    for (size_t i = 0; i <= 256; i++) {
        Codeword* cw = char_table[0][i];
        if (cw == nullptr) {
            continue;
        }
        PrefixTable* table = prefix_table;
        for (size_t i = 0; i < cw->size - 1; i++) {
            PrefixTable::PrefixTableEntry* entry = table->entries[cw->packedBits[i]];

            if (entry == nullptr) {
                entry = table->entries[cw->packedBits[i]] = new PrefixTable::PrefixTableEntry();
            }

            table = entry->nextTable;
            if (table == nullptr) {
                entry->nextTable = new PrefixTable();
                table = entry->nextTable;
            }
        }
        if (cw->lastBitsCount == 0) {
            int lastChunk = cw->packedBits[cw->size - 1];
            table->entries[lastChunk]->symbol = i;
            table->entries[lastChunk]->nextTable = nullptr;
            table->entries[lastChunk]->length = 0;
        } else {
            FillSymbolTable(i, cw->lastBitsCount, table, cw->packedBits[cw->size - 1], 7 - cw->lastBitsCount);
        }
    }
}


HuffmanCodec::HuffmanCodec() {
    tree = new HuffmanTree();

    for (size_t i = 0; i <= 8; i++) {
        for (size_t j = 0; j < 256 + 2; j++) {
            char_table[i][j] = nullptr;
        }
    }

    prefix_table = new PrefixTable();
}

HuffmanCodec::~HuffmanCodec() {
    if (tree != nullptr) {
        delete tree;
    }
    if (prefix_table != nullptr) {
        delete prefix_table;
    }
    for (size_t i = 0; i <= 8; i++) {
        for (size_t j = 0; j < 256 + 2; j++) {
            if (char_table[i][j] != nullptr) {
                delete char_table[i][j];
            }
        }
    }
}

void HuffmanCodec::learn(const vector<string>& vec) {
    for (const string& str : vec) {
        tree->LearnOnString(str);
    }
    tree->Build();
    GenerateCodes();
}

void HuffmanCodec::encode(const string_view& raw, string& encoded) const {
    encoded.reserve(2 * raw.size());
    // char* encoded = new char[2 * raw.size()];
    // char* ptr = encoded;
    uint8_t bitContainer = 0;
    size_t bitOffset = 0;
    for (size_t i = 0; i < raw.size() + 1; i++) {
        unsigned int ch;
        if (i != raw.size()) {
            ch = raw[i];
        } else {
            ch = 256;
        }
        Codeword* cw = char_table[bitOffset][ch];
        // std::cout << "merging codeword " << ch << " = " << *tree->char_table[0][ch] << ", offset is " << bitOffset << std::endl;
        bitContainer |= cw->packedBits[0];
        if (cw->size > 1) {
            // *ptr = bitContainer;
            // ptr++;
            encoded.push_back(bitContainer);
            for (size_t j = 1; j < cw->size - 1; j++) {
                encoded.push_back(cw->packedBits[j]);
            }
            // memcpy(ptr, cw->packedBits, (cw->size - 1) * sizeof(uint8_t));
            // ptr += cw->size - 2;
            bitContainer = cw->packedBits[cw->size - 1];
        }
        bitOffset = cw->lastBitsCount;
    }
    if (bitOffset > 0) {
        // *ptr = bitContainer;
        // ptr++;
        encoded.push_back(bitContainer);
    }

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

void HuffmanCodec::decode(const string_view& raw, string& result) const {
    result.reserve(2 * raw.size());
    int bitPos = 0;
    size_t pos = 1;
    uint8_t bitContainerLeft = raw[0];
    uint8_t bitContainerRight = 0;
    if (raw.size() > 1) {
        bitContainerRight = raw[1];
    }
    uint8_t bits = 0;
    bool decoding = true;
    PrefixTable* table = prefix_table;
    while (decoding) {
        bits = (bitContainerLeft << bitPos) | (bitContainerRight >> (8 - bitPos));
        // print_char_bits(bits); std::cout << " "; print_char_bits(bitContainerLeft); std::cout << " "; print_char_bits(bitContainerRight); std::cout << " " << bitPos << std::endl;
        PrefixTable::PrefixTableEntry* entry = table->entries[bits];
        if (entry->nextTable == nullptr) {
            int symbol = entry->symbol;
            if (symbol != 256) {
                result.push_back(symbol);
                bitPos += entry->length;
            } else {
                decoding = false;
                break;
            }
            table = prefix_table;
        } else {
            table = entry->nextTable;
            bitPos += 8;
        }
        if (bitPos >= 8) {
            bitPos -= 8;
            bitContainerLeft = raw[pos];
            if (pos + 1 < raw.size()) {
                bitContainerRight = raw[pos + 1];
            } else {
                bitContainerRight = 0;
            }
            pos++;
        }
    }
    // std::cout << std::endl;
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

    for (size_t i = 0; i <= 8; i++) {
        for (size_t j = 0; j < 256 + 2; j++) {
            if (char_table[i][j] != nullptr) {
                delete char_table[i][j];
                char_table[i][j] = nullptr;
            }
        }
    }

    if (prefix_table != nullptr) {
        delete prefix_table;
        prefix_table = new PrefixTable();
    }
}

}