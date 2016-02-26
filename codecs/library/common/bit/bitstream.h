#pragma once
#include <iostream>
#include <fstream>
#include "bit.h"

namespace Codecs {

    const unsigned int BIT_MASK[] = {
        0, 1, 3, 7, 15, 31, 63, 127, 255,
        511, 1023, 2047, 4095, 8191, 16383, 32767
    };

    class obitstream {
        private:
            const size_t BUFFER_SIZE = 64;

            std::ostream& stream;
            char* buffer;
            size_t bit_pos;

            void putBits(unsigned int bits, size_t byte_offset);
            void flushBuffer();

        public:
            obitstream(std::ostream& stream);

            void writeBit(bool bit);
            void writeBits(unsigned int bits, int nbits = 8);

            void dump();

            void flush();
    };

    class ibitstream {
        private:
            const size_t BUFFER_SIZE = 64;

            std::istream& stream;
            char* buffer;
            size_t bit_pos;

            void renewBuffer();
            unsigned int getBitsNoCheck(size_t nbits);

        public:
            ibitstream(std::istream& stream);

            bool getBit();
            unsigned int getBits(size_t nbits);
            void dump();
    };
}