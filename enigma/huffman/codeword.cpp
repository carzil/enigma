#include <iostream>

#include "enigma/huffman/codeword.h"

namespace Codecs {

Codeword::Codeword() {}

Codeword::Codeword(const std::vector<bool>& codeword, size_t bitOffset) {
    bitsCount = codeword.size() + bitOffset;
    size = bitsCount / 8;
    if (bitsCount % 8 != 0) {
        size += 1;
    }
    packedBits = new uint8_t[size];

    pack(codeword, bitOffset);
}

Codeword::~Codeword() {
    if (packedBits != nullptr) {
        delete[] packedBits;
    }
}

void Codeword::pack(const std::vector<bool>& bits, size_t bitOffset) {
    uint8_t bitContainer = 0;
    int bitPos = 7 - bitOffset;
    size_t pos = 0;
    for (size_t i = 0; i < bits.size(); i++) {
        if (bitPos == -1) {
            packedBits[pos] = bitContainer;
            pos++;
            bitPos = 7;
            bitContainer = 0;
        }
        bitContainer |= bits[i] << bitPos;
        bitPos--;
    }
    if (bitPos < 7) {
        packedBits[pos] = bitContainer;
        lastBitsCount = 7 - bitPos;
    }
}

std::ostream& operator<<(std::ostream& stream, const Codeword& codeword) {
    // std::cout << codeword.bitsCount << "{";
    for (size_t i = 0; i < codeword.bitsCount / 8; i++) {
        for (int bitPos = 7; bitPos >= 0; bitPos--) {
            bool bit = codeword.packedBits[i] & (1 << bitPos);
            stream << bit;
        }
        stream << "|";
    }
    if (codeword.bitsCount % 8 != 0) {
        int endPos = 7 - (codeword.bitsCount % 8);
        for (int bitPos = 7; bitPos > endPos; bitPos--) {
            bool bit = codeword.packedBits[codeword.bitsCount / 8] & (1 << bitPos);
            stream << bit;
        }
    }
    // std::cout << "}" << codeword.lastBitsCount;
    return stream;
}

}