#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
#include "huffman/tree.h"

namespace Codecs {

class HuffmanComparator {
    public:
        bool operator() (HuffmanNode*& lhs, HuffmanNode*& rhs) const {
            return lhs->frequency > rhs->frequency;
        }
};

HuffmanNode::HuffmanNode() : left(nullptr), right(nullptr), frequency(0) {}
HuffmanNode::HuffmanNode(HuffmanNode* left, HuffmanNode* right, size_t frequency) : left(left), right(right), frequency(frequency) {}
HuffmanNode::HuffmanNode(int c, size_t frequency) : left(nullptr), right(nullptr), c(c), frequency(frequency) {
    // std::cout << "created node c=" << c << ", frequency=" << frequency << std::endl;
}
HuffmanTree::HuffmanTree() : root(nullptr) {
    frequencies = new size_t[256 + 2];
    memset(frequencies, 0, sizeof(size_t) * (256 + 2));
}

HuffmanTree::~HuffmanTree() {
    DeleteTree();
    delete[] frequencies;
}

void HuffmanTree::LearnOnString(const std::string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        frequencies[static_cast<unsigned char>(str[i])]++;
    }
}

void HuffmanTree::BuildTree() {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanComparator> queue;

    queue.push(new HuffmanNode(256, 1));

    for (size_t i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            queue.push(new HuffmanNode(i, frequencies[i]));
        }
    }

    while (queue.size() >= 2) {
        // std::cout << queue.top()->c << " " << queue.top()->frequency << std::endl;
        HuffmanNode* left = queue.top(); 
        queue.pop();
        // std::cout << queue.top()->c << " " << queue.top()->frequency << std::endl;
        HuffmanNode* right = queue.top();
        queue.pop();

        // std::cout << "=======" << std::endl;
        HuffmanNode* node = new HuffmanNode(left, right, left->frequency + right->frequency);
        // printf("%d: #%d (%d), #%d (%d)\n", node->frequency, left->c, left->frequency, right->c, right->frequency);
        // std::cout << node->frequency << ": " << left->c << " " << right->c << std::endl;
        queue.push(node);
    }
    root = queue.top();
}

void HuffmanTree::Build() {
    // std::cout << "frequencies map: ";
    // for (size_t i = 0; i < 256; i++) {
    //     std::cout << frequencies[i] << " ";
    // }
    // std::cout << std::endl;
    if (root == nullptr) {
        BuildTree();
    }
}

void HuffmanTree::DeleteTree(HuffmanNode* node) {
    if (node->left == nullptr && node->right == nullptr) {
        delete node;
    } else {
        DeleteTree(node->left);
        DeleteTree(node->right);
        delete node;
    }
}

void HuffmanTree::DeleteTree() {
    DeleteTree(root);
    root = nullptr;
}

}