#pragma once

#include <cstdlib>
#include <string>

namespace Codecs {

struct HuffmanNode {
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

class HuffmanTree {
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