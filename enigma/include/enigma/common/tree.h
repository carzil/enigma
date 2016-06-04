#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include "enigma/common/codeword.h"
#include "enigma_export.h"

namespace Codecs {

class ENIGMA_NO_EXPORT HuffmanTree {
    public:
        struct Node {
            Node* left;
            Node* right;

            size_t c;
            size_t frequency;

            Node();
            Node(size_t c, size_t frequency);
            Node(Node* left, Node* right, size_t frequency);
            ~Node();

            bool operator<(const Node& other) const;
        };

        class Comparator {
            public:
                bool operator()(Node*, Node*);
        };

        Node* root;

        HuffmanTree();
        ~HuffmanTree();

        void PushValue(size_t value, size_t frequency);
        std::vector<std::pair<size_t, std::vector<bool>>> GenerateCodes();
        void Inorder();

        void Build();
        void Reset();

    private:
        std::vector<std::pair<size_t, size_t>> values;

        void GenerateCodes(Node* node, std::vector<bool>& bits, size_t depth, std::vector<std::pair<size_t, std::vector<bool>>>& result);
        void Inorder(Node*, size_t);
};

}