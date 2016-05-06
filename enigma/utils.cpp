#include <iostream>
#include "enigma/utils.h"

namespace Codecs {

void print_char_bits(char c) {
    int bitPos = 7;
    while (bitPos >= 0) {
        std::cout << bool(c & (1 << bitPos));
        bitPos--;
    }
}

}
