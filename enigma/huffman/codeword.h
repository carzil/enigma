#pragma once

#include <cstring>
#include <cstdint>
#include <vector>

namespace Codecs {

const size_t MAX_CODEWORD_LEGTH = 63;

class Codeword {
    private:
        void pack(const std::vector<bool>& bits, size_t bitOffset);

    public:
        uint8_t* packedBits;
        size_t lastBitsCount;
        size_t bitsCount;
        size_t size;

        Codeword();
        Codeword(const std::vector<bool>& codeword, size_t bitOffset);
        ~Codeword();

        friend std::ostream& operator<<(std::ostream& stream, const Codeword& codeword);

};

}