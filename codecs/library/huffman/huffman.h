#pragma once

#include <vector>
#include <library/common/codec.h>
#include <library/common/bit/bitstream.h>
#include <library/common/bit/bitstring.h>

namespace Codecs {

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

    class HuffmanTree {
        private:
            HuffmanNode* root;
            vector<size_t> frequencies; // chars + EOF
            vector<Codecs::bitstring*> char_table;

            void GenerateCodes(HuffmanNode* node, Codecs::bitstring* bs, int depth = 0);

        public:
            HuffmanTree();

            void LearnOnString(const string_view&);

            void GenerateCodes();
            void BuildTree();
            void EnsureBuilt();

            bitstring* Encode(unsigned char c);
            bitstring* EncodeEOF();
            int Decode(bitstring& stream);
            bool IsEOFCodeword(int codeword);
    };

    class HuffmanCodec : public CodecIFace {
        private:
            HuffmanTree* tree;

        public:
            HuffmanCodec();

            virtual void encode(string& encoded, string& raw) const override;
            virtual void decode(string& encoded, string& raw) const override;

            virtual string save() const override;
            virtual void load(const string_view&) override;

            virtual size_t sample_size(size_t records_total) const override;
            virtual void learn(const vector<string>& all_samples) override;
            virtual void learn(const string_view& view) override;

            virtual void reset() override;
    };
}
