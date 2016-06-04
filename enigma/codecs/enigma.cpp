#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <cstring>

#include "enigma/codecs/enigma.h"
#include "enigma/common/dictionary.h"
#include "enigma/colors.h"
#include "enigma/utils.h"


namespace Codecs {

void print_as_ints(const std::string& s) {
    for (unsigned char c : s) {
        std::cout << (int)c << " ";
    }
    std::cout << std::endl;
}

void EnigmaCodec::GenerateCodes() {
    vector<size_t> nodes;

    for (size_t i = 2; i < dict.Size(); i++) {
        nodes.push_back(i);
    }

    HuffmanTree tree;

    for (size_t i = 0; i < nodes.size(); i++) {
        if (frequencies[nodes[i]] > 0 || dict.GetNode(nodes[i]).parent == 1) {
            tree.PushValue(nodes[i], frequencies[nodes[i]]);
        }
    }
    tree.PushValue(endSymbol, frequencies[endSymbol]);
    tree.Build();
    auto pairs = tree.GenerateCodes();

    for (auto p : pairs) {
        codes[p.first] = new Codeword(p.second);
        prefix_table->AddCodeword(codes[p.first], p.first);
    }

    // PrintCodes();

}

EnigmaCodec::EnigmaCodec() {
    endSymbol = Dictionary::MAX_SIZE + 1;
    memset(frequencies, 0, sizeof(size_t) * (Dictionary::MAX_SIZE + 2));
    memset(codes, 0, sizeof(size_t) * (Dictionary::MAX_SIZE + 2));
    // memset(hist, 0, sizeof(size_t) * 1000);

    for (size_t c = 0; c <= 255; c++) {
        char ch = c;
        dict.AddNode(dict.GetRoot(), ch);
    }

    valid = 0;
    invalid = 0;

    prefix_table = new PrefixTable();
}

EnigmaCodec::~EnigmaCodec() {
    for (size_t i = 0; i < Dictionary::MAX_SIZE + 2; i++) {
        if (codes[i]) {
            delete codes[i];
        }
    }

    delete prefix_table;

    // for (size_t p : hist) {
    //     std::cout << p << " ";
    // }
    // std::cout << std::endl;
}

void EnigmaCodec::Learn(const vector<string>& vec) {
    for (const string& str : vec) {
        int ptr = dict.GetRoot();
        for (size_t i = 0; i < str.size(); i++) {
            if (dict.IsFull()) {
                break;
            }
            unsigned char c = str[i];
            int next = dict.NextNode(ptr, c);
            // std::cout << "symbol is '" << c << "'";
            if (dict.IsValid(next)) {
                // std::cout << ", in dict, go " << next << std::endl;
                ptr = next;
            } else {
                if (!dict.IsFull()) {
                    dict.AddNode(ptr, c);
                    // std::cout << ", not in dict, added string '" << GREEN(dict.RestoreString(p)) << "'" << std::endl;
                }
                ptr = dict.GetRoot();
            }
        }
    }
    for (const string& str : vec) {
        size_t ptr = dict.GetRoot();
        for (char c : str) {
            int next = dict.NextNode(ptr, c);
            // std::cout << ptr << " <-" << c << "-> " << next << std::endl;
            // std::cout << "symbol is '" << c << "'";
            if (dict.IsValid(next)) {
                // std::cout << ", in dict, ptr = " << ptr << ", go " << next << std::endl;
                ptr = next;
            } else {
                frequencies[ptr]++;
                // std::cout << ", not in dict, finished string '" << GREEN(dict.RestoreString(ptr)) << "', go root" << std::endl;
                ptr = dict.NextNode(dict.GetRoot(), c);
            }
        }

        // std::cout << "finished string '" << GREEN(dict.RestoreString(ptr)) << "', go root" << std::endl;

        frequencies[ptr]++;
    }

    frequencies[endSymbol] += vec.size();

    GenerateCodes();
}

void EnigmaCodec::WriteCodeword(char*& encoded, Codeword* cw, size_t& bitOffset, uint8_t& bitContainer) {
    bitContainer |= cw->packedBits[bitOffset][0];
    if (cw->size[bitOffset] > 1) {
        *encoded = bitContainer;
        encoded++;
        memcpy(encoded, cw->packedBits[bitOffset] + 1, cw->size[bitOffset] - 1);
        encoded += cw->size[bitOffset] - 2;
        bitContainer = cw->packedBits[bitOffset][cw->size[bitOffset] - 1];
    }
    bitOffset = cw->lastBitsCount[bitOffset];
}

size_t EnigmaCodec::encode(const string_view& raw, char* encoded) {
    uint8_t bitContainer = 0;
    size_t bitOffset = 0;
    const char* start = encoded;
    int ptr = dict.GetRoot();

    for (size_t i = 0; i < raw.size(); i++) {
        unsigned char c = raw[i];
        int next = dict.NextNode(ptr, c);
        // std::cout << (int)c << " ";
        if (dict.IsValid(next)) {
            ptr = next;
        } else {
            Codeword* cw = codes[ptr];
            WriteCodeword(encoded, cw, bitOffset, bitContainer);
            // std::cout << "merged codeword " << RED(*cw) << ", code = " << ptr << ", str = " << GREEN(dict.RestoreString(ptr)) << std::endl;
            // std::cout << std::endl << (int)c << " ";
            ptr = dict.NextNode(dict.GetRoot(), c);
        }
    }

    if (ptr != dict.GetRoot() && dict.IsValid(ptr)) {
        Codeword* cw = codes[ptr];
        // std::cout << "merged codeword " << RED(*cw) << ", code = " << ptr << ", str = " << GREEN(dict.RestoreString(ptr)) << std::endl;
        WriteCodeword(encoded, cw, bitOffset, bitContainer);
    }

    // std::cout << "merged codeword " << RED(*codes[endSymbol]) << ", code = " << GREEN("endSymbol") << std::endl;
    WriteCodeword(encoded, codes[endSymbol], bitOffset, bitContainer);

    if (bitOffset > 0) {
        *encoded = bitContainer;
        encoded++;
    }
    // for (const char* buf = start; buf != encoded; buf++) {
    //     print_char_bits(*buf); std::cout << " ";
    // }
    // std::cout << std::endl;
    return encoded - start;
}

size_t EnigmaCodec::decode(const string_view& raw, char* result) {
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
    const char* start = result;
    while (decoding) {
        bits = (bitContainerLeft << bitPos) | (bitContainerRight >> (8 - bitPos));
        // print_char_bits(bits); std::cout << " "; print_char_bits(bitContainerLeft); std::cout << " "; print_char_bits(bitContainerRight); std::cout << " " << bitPos << std::endl;
        PrefixTable::PrefixTableEntry* entry = table->entries[bits];
        if (entry->nextTable == nullptr) {
            int symbol = entry->symbol;
            if (symbol == endSymbol) {
                // std::cout << std::endl;
                decoding = false;
                break;
            }
            std::string s = inverse_codes[symbol];
            if (s.size() == 0) {
                inverse_codes[symbol] = dict.RestoreString(symbol);
            }
            s = inverse_codes[symbol];
            // std::cout << GREEN(s);
            memcpy(result, s.data(), s.size());
            result += s.size();
            table = prefix_table;
            bitPos += entry->length;
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
    return result - start;
}

void EnigmaCodec::save(DataOutput& out) const {
    std::string result;

    dict.Save(out);
    for (size_t i = 0; i < Dictionary::MAX_SIZE + 2; i++) {
        out.WriteInt(frequencies[i]);
        // std::cout << frequencies[i] << " ";
    }
    // std::cout << std::endl;

}

void EnigmaCodec::load(DataInput& in) {
    dict.Load(in);
    for (size_t i = 0; i < Dictionary::MAX_SIZE + 2; i++) {
        frequencies[i] = in.ReadInt();
        // std::cout << frequencies[i] << " ";
    }
    // std::cout << std::endl;
    GenerateCodes();
}

size_t EnigmaCodec::sample_size(size_t total) const {
    return total / 10;
}

void EnigmaCodec::reset() {
}

}
