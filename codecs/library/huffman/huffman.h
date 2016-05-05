#pragma once

#include <vector>
#include <cstdint>
#include <ostream>
#include <library/common/codec.h>
#include <library/common/bit/bitstream.h>

namespace Codecs {

    const size_t MAX_CODEWORD_LEGTH = 63;

    struct HuffmanNode {
        HuffmanNode* left;
        HuffmanNode* right;
        int c;
        size_t frequency;

        HuffmanNode();
        HuffmanNode(int c, size_t frequency);
        HuffmanNode(HuffmanNode* left, HuffmanNode* right, size_t frequency);

        bool operator<(const HuffmanNode& other);
    };

    struct Codeword {
        uint8_t* packedBits;
        size_t lastBitsCount;
        size_t bitsCount;
        size_t size;


        Codeword() {}

        Codeword(const vector<bool>& codeword, size_t bitOffset) {
            bitsCount = codeword.size() + bitOffset;
            size = bitsCount / 8;
            if (bitsCount % 8 != 0) {
                size += 1;
            }
            packedBits = new uint8_t[size];

            pack(codeword, bitOffset);
        }

        ~Codeword() {
        }

        void pack(const vector<bool>& bits, size_t bitOffset) {
            uint8_t bitContainer = 0;
            int bitPos = 7 - bitOffset;
            size_t pos = 0;
            for (size_t i = 0; i < bits.size(); i++) {
                if (bitPos == -1) {
                    packedBits[pos] = bitContainer;
                    pos++;
                    bitPos = 7;
                    bitContainer = 0;
                }
                bitContainer |= bits[i] << bitPos;
                bitPos--;
            }
            if (bitPos < 7) {
                packedBits[pos] = bitContainer;
                lastBitsCount = 7 - bitPos;
            }
        }

        friend std::ostream& operator<<(std::ostream& stream, const Codeword& codeword) {
            // std::cout << codeword.bitsCount << "{";
            for (size_t i = 0; i < codeword.bitsCount / 8; i++) {
                for (int bitPos = 7; bitPos >= 0; bitPos--) {
                    bool bit = codeword.packedBits[i] & (1 << bitPos);
                    stream << bit;
                }
                stream << "|";
            }
            if (codeword.bitsCount % 8 != 0) {
                int endPos = 7 - (codeword.bitsCount % 8);
                for (int bitPos = 7; bitPos > endPos; bitPos--) {
                    bool bit = codeword.packedBits[codeword.bitsCount / 8] & (1 << bitPos);
                    stream << bit;
                }
            }
            // std::cout << "}" << codeword.lastBitsCount;
            return stream;
        }
    };

    class PrefixTable {
        public: 
            struct PrefixTableEntry {
                PrefixTable* nextTable;
                int symbol;
                size_t length;

                PrefixTableEntry() : nextTable(nullptr), symbol(0), length(0) {}
            };

            PrefixTableEntry* entries[256];

            PrefixTable() {
                for (size_t i = 0; i < 256; i++) {
                    entries[i] = nullptr;
                }
            }
    };

    class HuffmanTree {
        private:
            size_t* frequencies; // chars + EOF
            void BuildTree();

        public:
            HuffmanNode* root;
            HuffmanTree();
            ~HuffmanTree();

            void LearnOnString(const string&);

            void Build();
    };

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
