#include "enigma/huffman/prefix_table.h"

namespace Codecs {

PrefixTable::PrefixTable() {
    for (size_t i = 0; i < 256; i++) {
        entries[i] = nullptr;
    }
}

PrefixTable::~PrefixTable() {
    for (size_t i = 0; i < 256; i++) {
        delete entries[i];
        entries[i] = nullptr;
    }
}
}