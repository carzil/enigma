#include <iostream>
#include <ostream>
#include <istream>
#include <cstring>
#include "bitstream.h"


namespace Codecs {
    obitbuffer::obitbuffer() {
    }

    obitbuffer::obitbuffer(char* buffer, size_t buffer_size) : buffer_ptr(buffer), buffer_start(buffer), bit_pos(0), current_bits(0), buffer_size(buffer_size) {
    }

    void obitbuffer::writeBits(size_t bits, size_t nbits) {
        current_bits ^= bits << bit_pos;
        bit_pos += nbits;
    }

    void obitbuffer::flush() {
        size_t bytes_cnt = bit_pos >> 3;
        *(size_t*)buffer_ptr = current_bits;
        buffer_ptr += bytes_cnt;
        bit_pos &= 7;
        current_bits >>= bytes_cnt << 3;
    }

    size_t obitbuffer::size() {
        return buffer_ptr - buffer_start;
    }
}