#include <iostream>
#include <ostream>
#include <istream>
#include <cstring>
#include "bitstream.h"


namespace Codecs {
    obitstream::obitstream(std::ostream& stream) : stream(stream) {
        buffer = new char[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        bit_pos = 0;
    }

    void obitstream::writeBit(bool bit) {
        writeBits(bit, 1);
    }

    void obitstream::writeBits(unsigned int bits, int nbits) {
        // bits &= 0xffff >> (16 - nbits);
        size_t byte_offset = bit_pos >> 3;
        if (bit_pos + nbits >= BUFFER_SIZE * 8) {
            size_t cnt = BUFFER_SIZE * 8 - bit_pos;
            bits &= 0xffff >> (16 - cnt);
            putBits(bits, byte_offset);
            flushBuffer();
            bits = bits >> (BUFFER_SIZE * 8 - bit_pos);
            nbits -= BUFFER_SIZE * 8 - bit_pos;
            bit_pos = 0;
            byte_offset = 0;
        }
        putBits(bits, byte_offset);
        bit_pos += nbits;
    }

    inline void obitstream::putBits(unsigned int bits, size_t byte_offset) {
        std::cout << "bit_pos = " <<  bit_pos << ", byte offset = " << byte_offset << ", bits = " << (bits << (bit_pos & 0xf));
        std::cout << ", bits_raw = " << bits << std::endl;
        byte_offset &= ~1;
        *(unsigned int*)(buffer + byte_offset) |= bits << (bit_pos & 0xf);
        // dump();
    }

    void obitstream::flushBuffer() {
        stream.write(buffer, BUFFER_SIZE);
        memset(buffer, '\0', BUFFER_SIZE);
    }

    void obitstream::flush() {
        size_t byte_offset = bit_pos >> 3;
        // std::cout << "byte_offset = " << ((byte_offset & ~4) << 2) << std::endl;
        stream.write(buffer, byte_offset + 4);
        stream.flush();
        bit_pos = 0;
    }

    void obitstream::dump() {
        for (size_t i = 0; i <= 20; i++) {
            // std::cout << *((unsigned int*)(buffer) + i) << " ";
            std::cout << (int)((unsigned char)buffer[i]) << " ";
        }
        std::cout << std::endl;
    }

    ibitstream::ibitstream(std::istream& stream) : stream(stream) {
        bit_pos = 0;
        buffer = new char[BUFFER_SIZE];
        renewBuffer();
    }

    void ibitstream::renewBuffer() {
        memset(buffer, '\0', BUFFER_SIZE);
        stream.read(buffer, BUFFER_SIZE);
    }

    void ibitstream::dump() {
        for (size_t i = 0; i <= 20; i++) {
            // std::cout << *((unsigned int*)(buffer) + i) << " ";
            std::cout << (int)((unsigned char)buffer[i]) << " ";
        }
        std::cout << std::endl;
    }

    bool ibitstream::getBit() {
        return getBits(1);
    }

    unsigned int ibitstream::getBits(size_t nbits) {
        if (bit_pos + nbits >= BUFFER_SIZE * 8) {
            unsigned int result = 0;
            size_t cnt = BUFFER_SIZE * 8 - bit_pos;
            std::cout << "cnt = " << cnt << std::endl;
            result = getBitsNoCheck(cnt) << cnt;
            renewBuffer();
            bit_pos = 0;
            result |= getBitsNoCheck(nbits - cnt);
            return result;
        } else {
            return getBitsNoCheck(nbits);
        }
    }

    unsigned int ibitstream::getBitsNoCheck(size_t nbits) {
        unsigned int byte_offset = bit_pos >> 3;
        byte_offset &= ~1;
        unsigned int bits = *(unsigned int*)(buffer + byte_offset);
        // std::cout << "bit_pos = " << bit_pos << ", bits_b = " << bits << ", ";
        bits >>= bit_pos & 0xf;
        // std::cout << "bits_a = " << bits << ", shift = " << (bit_pos & 0xf) << ", bit_mask = " << BIT_MASK[nbits] << std::endl;
        bit_pos += nbits;
        return bits & BIT_MASK[nbits];
    }
}