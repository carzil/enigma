#include <algorithm>
#include "bit.h"

namespace Codecs {
    bitwriter::bitwriter() {
        char_pos = 7;
        current_char = 0;
    }

    bitwriter::bitwriter(unsigned char current_char, int char_pos) : char_pos(char_pos), current_char(current_char) {

    }

    int bitwriter::writeBit(bool bit) {
        fixCharPosition();
        if (bit) {
            current_char |= 1 << char_pos;
        } else {
            current_char &= ~(1 << char_pos);
        }
        char_pos--;
    }

    int bitwriter::writeBits(unsigned char bits, int nbits) {
        fixCharPosition();
        bits &= (~0 << (8 - nbits));
        if (char_pos == 7) {
            current_char = bits;
            char_pos = 7 - nbits;
        } else {
            int len = std::min(char_pos + 1, nbits);
            current_char |= (bits >> (7 - char_pos));
            char_pos -= len;
            if (char_pos < 0) {
                appendChar(current_char);
                current_char = 0;
                current_char |= (bits << len);
                len = std::max(nbits - len, 0);
                char_pos = 7 - len;
            }
        }
    }

    void bitwriter::fixCharPosition() {
        if (char_pos == -1) {
            appendChar(current_char);
            current_char = 0;
            char_pos = 7;
        }
    }

    void bitwriter::flush() {
        appendChar(current_char);
        current_char = 0;
    }
}