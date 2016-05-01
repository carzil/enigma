#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include "huffman.h"


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
    HuffmanTree::HuffmanTree() : root(nullptr), frequencies(256 + 1), char_table(256 + 1) {
    }

    void pr(std::vector<char>& v, size_t pos) {
        string p;
        for (size_t i = 0; i < pos; i++) {
            p.push_back(v[i]);
        }
    }

    void HuffmanTree::GenerateCodes(HuffmanNode* node, bitstring* bs, int depth) {
        // std::cout << pos << std::endl;
        if (node == nullptr) {
            return;
        }
        bs->writeBit(0);
        GenerateCodes(node->left, bs, depth + 1);
        bs->revert();

        bs->writeBit(1);
        GenerateCodes(node->right, bs, depth + 1);
        bs->revert();

        if (node->left == nullptr && node->right == nullptr) {
            char_table[node->c] = bs->clone();
            std::cout << "code for '" << (unsigned char)node->c << "' (" << node->c << ") is " << char_table[node->c]->format();
            std::cout << " (size = " << bs->sizeInBits() << "), depth is " << depth << std::endl;
            // std::cout << char_table[node->c]->format() << std::endl;
        }
    }

    void HuffmanTree::GenerateCodes() {
        string* s = new string();
        GenerateCodes(root, new bitstring(s));
    }

    void HuffmanTree::LearnOnString(const string_view& str) {
        for (size_t i = 0; i < str.size(); i++) {
            frequencies[static_cast<unsigned char>(str[i])]++;
        }
    }

    void HuffmanTree::BuildTree() {
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanComparator> queue;

        for (size_t i = 0; i < 257; i++) {
            if (i == 256 || frequencies[i] > 0) {
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

    bitstring* HuffmanTree::Encode(unsigned char c) {
        return char_table[c];
    }

    bitstring* HuffmanTree::EncodeEOF() {
        return char_table[256];
    }

    bool HuffmanTree::IsEOFCodeword(int codeword) {
        return codeword == 256;
    }

    void HuffmanTree::EnsureBuilt() {
        if (root == nullptr) {
            BuildTree();
            GenerateCodes();
        }
    }

    int HuffmanTree::Decode(bitstring& stream) {
        HuffmanNode* current_node = root;
        while (!(current_node->left == nullptr && current_node->right == nullptr)) {
            bool bit = stream.getBit();
            // std::cout << bit;
            if (bit) {
                current_node = current_node->right;
            } else {
                current_node = current_node->left;
            }
        }
        // std::cout << "|";
        return current_node->c;
    }

    HuffmanCodec::HuffmanCodec() {
        tree = new HuffmanTree();
    }

    void HuffmanCodec::learn(const vector<string>& vec) {
        for (const string_view& str : vec) {
            tree->LearnOnString(str);
        }
    }

    void HuffmanCodec::learn(const string_view& view) {
        tree->LearnOnString(view);
    }

    void HuffmanCodec::encode(string& result, string& raw) const {
        tree->EnsureBuilt();
        bitstring encoded(&result);
        for (size_t i = 0; i < raw.size(); i++) {
            bitstring* codeword = tree->Encode(raw[i]);
            // std::cout << "merging codeword " << codeword->format() << " (" << raw[i] << ")" << std::endl;
            encoded.merge(*codeword);
        }
        encoded.merge(*tree->EncodeEOF());
        // std::cout << encoded.format() << std::endl;
        encoded.flush();
    }

    void HuffmanCodec::decode(string& result, string& raw) const {
        tree->EnsureBuilt();
        bitstring encoded(&result);
        int codeword = tree->Decode(encoded);
        raw.reserve(100);
        while (!tree->IsEOFCodeword(codeword)) {
            raw.push_back(static_cast<unsigned char>(codeword));
            codeword = tree->Decode(encoded);
        }
        raw.shrink_to_fit();
    }

    string HuffmanCodec::save() const {
        return string();
    }

    void HuffmanCodec::load(const string_view&) {
    }

    size_t HuffmanCodec::sample_size(size_t total) const {
        return total / 10;
    }

    void HuffmanCodec::reset() {
    }

}
