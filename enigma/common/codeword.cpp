#include <iostream>

#include "enigma/common/codeword.h"

namespace Codecs {

Codeword::Codeword() {}

Codeword::Codeword(const std::vector<bool>& codeword) {
    for (size_t bitOffset = 0; bitOffset <= 8; bitOffset++) {
        bitsCount[bitOffset] = codeword.size() + bitOffset;
        size[bitOffset] = bitsCount[bitOffset] / 8;
        if (bitsCount[bitOffset] % 8 != 0) {
            size[bitOffset] += 1;
        }
        packedBits[bitOffset] = new uint8_t[size[bitOffset]];
        pack(codeword, bitOffset);
    }
}

Codeword::~Codeword() {
    for (int i = 0; i <= 8; i++) {
        if (packedBits[i] != nullptr) {
            delete[] packedBits[i];
        }
    }
}

void Codeword::pack(const std::vector<bool>& bits, size_t bitOffset) {
    uint8_t bitContainer = 0;
    int bitPos = 7 - bitOffset;
    size_t pos = 0;
    for (size_t i = 0; i < bits.size(); i++) {
        if (bitPos == -1) {
            packedBits[bitOffset][pos] = bitContainer;
            pos++;
            bitPos = 7;
            bitContainer = 0;
        }
        bitContainer |= bits[i] << bitPos;
        bitPos--;
    }
    if (bitPos < 7) {
        packedBits[bitOffset][pos] = bitContainer;
        lastBitsCount[bitOffset] = 7 - bitPos;
    }
}

std::ostream& operator<<(std::ostream& stream, const Codeword& codeword) {
    // std::cout << codeword.bitsCount << "{";
    for (size_t i = 0; i < codeword.bitsCount[0] / 8; i++) {
        for (int bitPos = 7; bitPos >= 0; bitPos--) {
            bool bit = codeword.packedBits[0][i] & (1 << bitPos);
            stream << bit;
        }
        stream << "|";
    }
    if (codeword.bitsCount[0] % 8 != 0) {
        int endPos = 7 - (codeword.bitsCount[0] % 8);
        for (int bitPos = 7; bitPos > endPos; bitPos--) {
            bool bit = codeword.packedBits[0][codeword.bitsCount[0] / 8] & (1 << bitPos);
            stream << bit;
        }
    }
    // std::cout << "}" << codeword.lastBitsCount;
    return stream;
}

}