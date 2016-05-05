#pragma once

#include <cstdint>
#include <ostream>
#include <memory>
#include <vector>

#include "common/codec.h"
#include "huffman/tree.h"
#include "huffman/codeword.h"
#include "huffman/prefix_table.h"

namespace Codecs {

class HuffmanCodec : public CodecIFace {
    private:
        HuffmanTree* tree;
        Codeword* char_table[9][258];
        PrefixTable* prefix_table;

        void GenerateCodes(HuffmanNode* node, vector<bool>& codeword, size_t depth);
        void GenerateCodes();

        void FillSymbolTable(int symbol, size_t length, PrefixTable* table, int lastChunk, int pos);

    public:
        HuffmanCodec();
        ~HuffmanCodec();

        virtual void encode(const string_view&, string&) const override;
        virtual void decode(const string_view&, string&) const override;

        virtual string save() const override;
        virtual void load(const string_view&) override;

        virtual size_t sample_size(size_t records_total) const override;
        virtual void learn(const vector<string>& all_samples) override;

        virtual void reset() override;
};
}
