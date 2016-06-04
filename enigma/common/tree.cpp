#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

#include "enigma/common/tree.h"
#include "enigma/colors.h"

namespace Enigma {

HuffmanTree::Node::Node() : left(nullptr), right(nullptr), c(0), frequency(0) {}
HuffmanTree::Node::Node(HuffmanTree::Node* left, HuffmanTree::Node* right, size_t frequency) : left(left), right(right), c(0), frequency(frequency) {}
HuffmanTree::Node::Node(size_t c, size_t frequency) : left(nullptr), right(nullptr), c(c), frequency(frequency) {}

bool HuffmanTree::Comparator::operator()(HuffmanTree::Node* left, HuffmanTree::Node* right) {
    return left->frequency > right->frequency;
}

HuffmanTree::Node::~Node() {
    if (left != nullptr) {
        delete left;
    }

    if (right != nullptr) {
        delete right;
    }
}

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    delete root;
}

void HuffmanTree::PushValue(size_t value, size_t frequency) {
    values.push_back({ value, frequency });
}

void HuffmanTree::Build() {
    if (root != nullptr) {
        return;
    }

    std::priority_queue<HuffmanTree::Node*, std::vector<HuffmanTree::Node*>, Comparator> queue;
    for (auto p : values) {
        queue.push(new HuffmanTree::Node(p.first, p.second));
    }

    while (queue.size() >= 2) {
        // std::cout << queue.top()->c << " " << queue.top()->frequency << std::endl;
        HuffmanTree::Node* left = queue.top(); 
        queue.pop();
        // std::cout << queue.top()->c << " " << queue.top()->frequency << std::endl;
        HuffmanTree::Node* right = queue.top();
        queue.pop();

        // std::cout << "=======" << std::endl;
        HuffmanTree::Node* node = new HuffmanTree::Node(left, right, left->frequency + right->frequency);
        // printf("%d: #%d (%d), #%d (%d)\n", node->frequency, left->c, left->frequency, right->c, right->frequency);
        // std::cout << node->frequency << ": " << left->c << " " << right->c << std::endl;
        queue.push(node);
    }
    root = queue.top();
}

void HuffmanTree::Inorder() {
    Inorder(root, 0);
}

void HuffmanTree::Inorder(HuffmanTree::Node* node, size_t indent) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < indent; i++) {
        std::cout << ".";
    }
    std::cout << "(" << node->c << ", " << node->frequency << ")" << std::endl;
    Inorder(node->left, indent + 2);
    Inorder(node->right, indent + 2);
}

void HuffmanTree::Reset() {
    delete root;
    root = nullptr;
}

std::vector<std::pair<size_t, std::vector<bool>>> HuffmanTree::GenerateCodes() {
    std::vector<std::pair<size_t, std::vector<bool>>> result;
    std::vector<bool> bits;
    HuffmanTree::GenerateCodes(root, bits, 0, result);
    return result;
}

void HuffmanTree::GenerateCodes(HuffmanTree::Node* node, std::vector<bool>& bits, size_t depth, std::vector<std::pair<size_t, std::vector<bool>>>& result) {
    if (node == nullptr) {
        return;
    }

    if (node->left == nullptr && node->right == nullptr) {
        result.push_back({ node->c, bits });
        // std::cout << "code for " << GREEN(node->c) << " (" << GREEN(static_cast<uint8_t>(node->c)) << ")" << " is " << RED(*(new Codeword(bits))) << std::endl;
    } else {
        bits.push_back(0);
        GenerateCodes(node->left, bits, depth + 1, result);
        bits.back() = 1;
        GenerateCodes(node->right, bits, depth + 1, result);
        bits.resize(bits.size() - 1);
    }
}

}