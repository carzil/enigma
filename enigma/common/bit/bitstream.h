#pragma once
#include <iostream>
#include <fstream>
#include "bit.h"

namespace Codecs {

    class obitbuffer {
        private:

            char* buffer_ptr;
            char* buffer_start;

            size_t bit_pos;
            size_t current_bits;
            size_t buffer_size;
            
        public:
            obitbuffer();
            obitbuffer(char* buffer, size_t buffer_size);

            void writeBit(bool bit);
            void writeBits(size_t bits, size_t nbits = 8);
            void flush();
            
            size_t size();
    };
}