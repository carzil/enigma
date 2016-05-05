#pragma once

#include <cstdlib>

namespace Codecs {

class PrefixTable {
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
};

}