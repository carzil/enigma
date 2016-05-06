#pragma once

#include <cstdlib>
#include <string>
#include "enigma_export.h"

namespace Codecs {

struct ENIGMA_NO_EXPORT HuffmanNode {
    HuffmanNode* left;
    HuffmanNode* right;
    int c;
    size_t frequency;

    HuffmanNode();
    HuffmanNode(int c, size_t frequency);
    HuffmanNode(HuffmanNode* left, HuffmanNode* right, size_t frequency);
    ~HuffmanNode();

    bool operator<(const HuffmanNode& other);
};

class ENIGMA_NO_EXPORT HuffmanTree {
    private:
        size_t* frequencies; // chars + EOF

        void BuildTree();

    public:
        HuffmanNode* root;
        HuffmanTree();
        ~HuffmanTree();

        void LearnOnString(const std::string&);
        void Build();
        void Reset();
};

}