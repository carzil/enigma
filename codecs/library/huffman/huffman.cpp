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

    void HuffmanTree::GenerateCodes(HuffmanNode* node, Codecs::Bitstring* bs, int depth) {
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
            std::cout << "code for '" << (char)node->c << "' (" << node->c << ") is " << char_table[node->c]->format();
            std::cout << " (size = " << bs->sizeInBits() << "), depth is " << depth << std::endl;
            // std::cout << char_table[node->c]->format() << std::endl;
        }
    }

    void HuffmanTree::GenerateCodes() {
        string* s = new string();
        GenerateCodes(root, new Bitstring(s));
    }

    void HuffmanTree::LearnOnString(const string_view& str) {
        for (size_t i = 0; i < str.size(); i++) {
            frequencies[str[i]] += 1;
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

    Bitstring* HuffmanTree::Encode(char c) {
        return char_table[c];
    }

    Bitstring* HuffmanTree::EncodeEOF() {
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

    inline int HuffmanTree::Decode(ibitstream& stream) {
        HuffmanNode* current_node = root;
        while (!(current_node->left == nullptr && current_node->right == nullptr)) {
            bool bit = stream.getBit();
            if (bit) {
                current_node = current_node->right;
            } else {
                current_node = current_node->left;
            }
        }
        // std::cout << (char)current_node->c;
        return current_node->c;
    }

    HuffmanCodec::HuffmanCodec() {
        tree = new HuffmanTree();
    }

    void HuffmanCodec::learn(const StringViewVector& vec) {
        for (const string_view& str : vec) {
            tree->LearnOnString(str);
        }
    }

    void HuffmanCodec::learn(const string_view& view) {
        tree->LearnOnString(view);
    }

    void HuffmanCodec::encodeChunk(obitstream& encoded, const string_view& raw) const {
        tree->EnsureBuilt();
        for (char current_char : raw) {
            Codecs::Bitstring* codeword = tree->Encode(current_char);
            if (true || codeword->sizeInBits() > 8) {
                unsigned int bits = ((unsigned int)current_char << 1) | 1;
                // std::cout << "merging char '" << current_char << "' (" << (int)current_char << ") as-is (" << bits << ")" << std::endl;
                encoded.writeBits(bits, 9);
            } else {
                // std::cout << "merging codeword " << codeword->format() << " (" << current_char << ")" << std::endl;
                encoded.writeBit(0);
                codeword->write(encoded);
            }
            // std::cout << codeword->format() << " | ";
        }
        // std::cout << "resulting string: " << bs.format() << std::endl;
        // bs.flush();
        // for (size_t i = 0; i < encoded.size(); i++) {
        //     printf("%d ", (unsigned char)encoded.at(i));
        // }
        // printf("\n");
    }

    void HuffmanCodec::stopEncoding(obitstream& encoded) const {
        encoded.writeBit(0);
        tree->EncodeEOF()->write(encoded);
    }

    void HuffmanCodec::decode(string& raw, ibitstream& encoded) const {
        while (true) {
            // int codeword = tree->Decode(encoded);
            // while (!tree->IsEOFCodeword(codeword)) {
            //     // std::cout << (char)codeword;
            //     codeword = tree->Decode(encoded);
            // }
            bool is_pruned = encoded.getBit();
            // std::cout << "pruned = " << is_pruned << std::endl;
            if (is_pruned) {
                unsigned int decoded = (unsigned int)encoded.getBits(8);
                // std::cout << "decoded '" << decoded << "'" << std::endl;
                raw.push_back(decoded);
            } else {
                int decoded = tree->Decode(encoded);
                if (tree->IsEOFCodeword(decoded)) {
                    break;
                } else {
                    raw.push_back((char)decoded);
                }
            }
        }
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
