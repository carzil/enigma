#pragma once

#include <cstdint>
#include <ostream>
#include <memory>
#include <vector>

#include "enigma/common/codec.h"
#include "enigma/common/codeword.h"
#include "enigma/common/dictionary.h"
#include "enigma/huffman/tree.h"
#include "enigma/huffman/prefix_table.h"
#include "enigma_export.h"

namespace Codecs {

class ENIGMA_API EnigmaCodec : public CodecIFace {
    private:
        Dictionary dict;
        size_t frequencies[Dictionary::MAX_SIZE + 2];
        Codeword* codes[Dictionary::MAX_SIZE + 2];
        std::string inverse_codes[Dictionary::MAX_SIZE];
        PrefixTable* prefix_table;
        size_t endSymbol;
        size_t valid;
        size_t invalid;

        void GenerateCodes();
        void WriteCodeword(char*& encoded, Codeword* cw, size_t& bitOffset, uint8_t& bitContainer);

    public:
        EnigmaCodec();
        ~EnigmaCodec();

        virtual size_t encode(const string_view&, char*) override;
        virtual size_t decode(const string_view&, char*) override;

        virtual string save() const override;
        virtual void load(const string_view&) override;

        virtual size_t sample_size(size_t records_total) const override;
        virtual void Learn(const vector<string>& samples) override;
        virtual void Learn(const string& samples) override;
        virtual void EndLearning() override;

        virtual void reset() override;
};
}
