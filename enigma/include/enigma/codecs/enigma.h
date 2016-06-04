#pragma once

#include <cstdint>
#include <ostream>
#include <memory>
#include <vector>

#include "enigma/common/codec.h"
#include "enigma/common/codeword.h"
#include "enigma/common/dictionary.h"
#include "enigma/common/tree.h"
#include "enigma/common/prefix_table.h"
#include "enigma_export.h"
#include "enigma/colors.h"

namespace Enigma {

class ENIGMA_API EnigmaCodec : public CodecIFace {
    private:
        Dictionary dict;
        size_t frequencies[Dictionary::MAX_SIZE + 2];
        Codeword* codes[Dictionary::MAX_SIZE + 2];
        std::string inverse_codes[Dictionary::MAX_SIZE];
        PrefixTable* prefix_table;
        int endSymbol;
        size_t valid;
        size_t invalid;

        void GenerateCodes();
        void WriteCodeword(char*& encoded, Codeword* cw, size_t& bitOffset, uint8_t& bitContainer);

        class cmp {
            size_t* frequencies;

            public:
                cmp(size_t* frequencies) : frequencies(frequencies) {}

                bool operator()(size_t a, size_t b) {
                    return frequencies[a] > frequencies[b];
                }
        };

    public:
        EnigmaCodec();
        ~EnigmaCodec();

        virtual size_t encode(const string_view&, char*) override;
        virtual size_t decode(const string_view&, char*) override;

        virtual void save(DataOutput&) const override;
        virtual void load(DataInput&) override;

        virtual size_t sample_size(size_t records_total) const override;
        virtual void Learn(const vector<string>& samples) override;

        virtual void reset() override;

        void PrintCodes() {
            vector<size_t> nodes;

            for (size_t i = 2; i < dict.Size(); i++) {
                nodes.push_back(i);
            }

            cmp c(frequencies);


            sort(nodes.begin(), nodes.end(), c);
            reverse(nodes.begin(), nodes.end());

            size_t cnt = 0;

            for (size_t i = 0; i < nodes.size(); i++) {
                size_t ptr = nodes[i];
                Codeword* cw = codes[ptr];
                size_t matches = frequencies[ptr];
                if (cw && matches > 0) {
                    std::string str = dict.RestoreString(ptr);
                    std::cout << "code for '" << GREEN(str) << "' (ptr = " << ptr << ") is " << RED(*cw);
                    size_t originSize = str.size() * 8;
                    size_t comprSize = cw->bitsCount[0];
                    std::cout << " (" << originSize << " bits -> " <<  comprSize << " bits, " << matches << " matches, ";
                    if (originSize < comprSize) {
                        std::cout << REDB("lost " << abs(originSize - comprSize) * matches / 8 << " bytes");
                    } else {
                        std::cout << GREENB("won " << abs(originSize - comprSize) * matches / 8 << " bytes");
                    }
                    std::cout << ")" << std::endl;
                    cnt++;
                }
            }
            std::cout << "code for " << GREEN("endSymbol") << ": " << RED(*codes[endSymbol]) << std::endl;
            // cnt *= 4 + 1;
            // std::cout << "dictionary memory: " << cnt << " bytes (" << cnt / 1024 / 1024 << " MBytes)" << std::endl;
            // cnt = 0;
            // std::cout << "leaf nodes: " << cnt << std::endl;
            // std::cout << "internal nodes: " << dict.Size() - cnt << std::endl; 
        }
};
}
