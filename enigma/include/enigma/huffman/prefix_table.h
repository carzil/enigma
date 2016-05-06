#pragma once

#include <cstdlib>
#include "enigma/huffman/codeword.h"
#include "enigma_export.h"

namespace Codecs {

class ENIGMA_NO_EXPORT PrefixTable {
    private:
        void FillSymbolTable(int symbol, size_t length, PrefixTable* table, int lastChunk, int pos);

    public: 
        struct PrefixTableEntry {
            PrefixTable* nextTable;
            int symbol;
            size_t length;

            PrefixTableEntry() : nextTable(nullptr), symbol(0), length(0) {}
            ~PrefixTableEntry() {
                if (nextTable != nullptr) {
                    delete nextTable;
                }
            }
        };

        PrefixTableEntry* entries[256];

        PrefixTable();
        ~PrefixTable();

        void AddCodeword(Codeword*, int);
};

}