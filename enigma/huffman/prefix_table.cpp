#include "enigma/huffman/prefix_table.h"
#include "enigma/utils.h"

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

void PrefixTable::AddCodeword(Codeword* cw, int symbol) {
    PrefixTable* table = this;
    for (size_t i = 0; i < cw->size - 1; i++) {
        PrefixTable::PrefixTableEntry* entry = table->entries[cw->packedBits[i]];

        if (entry == nullptr) {
            entry = table->entries[cw->packedBits[i]] = new PrefixTable::PrefixTableEntry();
        }

        table = entry->nextTable;
        if (table == nullptr) {
            entry->nextTable = new PrefixTable();
            table = entry->nextTable;
        }
    }
    if (cw->lastBitsCount == 0) {
        int lastChunk = cw->packedBits[cw->size - 1];
        table->entries[lastChunk]->symbol = symbol;
        table->entries[lastChunk]->nextTable = nullptr;
        table->entries[lastChunk]->length = 0;
    } else {
        FillSymbolTable(symbol, cw->lastBitsCount, table, cw->packedBits[cw->size - 1], 7 - cw->lastBitsCount);
    }
}

void PrefixTable::FillSymbolTable(int symbol, size_t length, PrefixTable* table, int lastChunk, int pos) {
    if (pos == -1) {
        table->entries[lastChunk] = new PrefixTable::PrefixTableEntry();
        table->entries[lastChunk]->symbol = symbol;
        table->entries[lastChunk]->length = length;
        table->entries[lastChunk]->nextTable = nullptr;
        // print_char_bits(lastChunk); std::cout << " -> '" << (char)symbol << "' (" << symbol << "), execess = " << length << std::endl;
    } else {
        FillSymbolTable(symbol, length, table, lastChunk | (1 << pos), pos - 1);
        FillSymbolTable(symbol, length, table, lastChunk, pos - 1);
    }
}

}