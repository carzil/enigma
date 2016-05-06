#pragma once

#include <cstdint>
#include <ostream>
#include <memory>
#include <vector>

#include "enigma/common/codec.h"
#include "enigma/huffman/tree.h"
#include "enigma/huffman/codeword.h"
#include "enigma/huffman/prefix_table.h"
#include "enigma_export.h"

namespace Codecs {

class ENIGMA_API HuffmanCodec : public CodecIFace {
    private:
        HuffmanTree* tree;
        Codeword* char_table[9][258];
        PrefixTable* prefix_table;

        void GenerateCodes(HuffmanNode* node, vector<bool>& codeword, size_t depth);
        void GenerateCodes();

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
