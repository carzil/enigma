#pragma once

#include <cstring>
#include <cstdint>
#include <iostream>
#include <vector>

#include "enigma_export.h"

namespace Codecs {

const size_t MAX_CODEWORD_LEGTH = 63;

class ENIGMA_NO_EXPORT Codeword {
    public:
        uint8_t* packedBits[9];
        size_t lastBitsCount[9];
        
        size_t bitsCount[9];
        size_t size[9];

        Codeword();
        Codeword(const std::vector<bool>& codeword);
        ~Codeword();

        friend std::ostream& operator<<(std::ostream& stream, const Codeword& codeword);

    private:
        void pack(const std::vector<bool>& bits, size_t bitOffset);

};

}