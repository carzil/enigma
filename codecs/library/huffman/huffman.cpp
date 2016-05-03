#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <cstring>
#include "library/common/bit/bitstream.h"
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
    HuffmanTree::HuffmanTree() : root(nullptr) {
        frequencies = new size_t[256 + 2];
        memset(frequencies, 0, sizeof(size_t) * (256 + 2));
        for (size_t i = 0; i <= 8; i++) {
            char_table[i] = new Codeword*[256 + 2];
        }
    }

    HuffmanTree::~HuffmanTree() {
        delete[] frequencies;
        for (size_t i = 0; i <= 8; i++) {
            delete[] char_table[i];
        }
    }

    void pr(std::vector<char>& v, size_t pos) {
        string p;
        for (size_t i = 0; i < pos; i++) {
            p.push_back(v[i]);
        }
    }

    void HuffmanTree::GenerateCodes(HuffmanNode* node, vector<bool>& bits, size_t depth) {
        // std::cout << pos << std::endl;
        if (node == nullptr) {
            return;
        }

        bits.push_back(0);
        GenerateCodes(node->left, bits, depth + 1);
        bits.back() = 1;
        GenerateCodes(node->right, bits, depth + 1);
        bits.resize(bits.size() - 1);

        if (node->left == nullptr && node->right == nullptr) {
            for (size_t bitOffset = 0; bitOffset <= 8; bitOffset++) {
                Codeword* cw = new Codeword(bits, bitOffset);
                char_table[bitOffset][node->c] = cw;
            }
            std::cout << "code for '" << (char)node->c << "' (";
            std::cout << node->c << ") is " << *char_table[0][node->c] << ", depth = " << depth << std::endl;
            // std::cout << "======" << std::endl;
        }
    }

    void HuffmanTree::GenerateCodes() {
        vector<bool> bits;
        GenerateCodes(root, bits, 0);
    }

    void HuffmanTree::LearnOnString(const string& str) {
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
        std::cout << "frequencies map: ";
        for (size_t i = 0; i < 256; i++) {
            std::cout << frequencies[i] << " ";
        }
        std::cout << std::endl;
        if (root == nullptr) {
            clock_t begin = clock();
            BuildTree();
            GenerateCodes();
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            std::cout << "tree builded in " << elapsed_secs << " secs" << std::endl;
        }
    }

    // int HuffmanTree::Decode(bitstring& stream) {
    //     HuffmanNode* current_node = root;
    //     while (!(current_node->left == nullptr && current_node->right == nullptr)) {
    //         bool bit = stream.getBit();
    //         // std::cout << bit;
    //         if (bit) {
    //             current_node = current_node->right;
    //         } else {
    //             current_node = current_node->left;
    //         }
    //     }
    //     // std::cout << "|";
    //     if (current_node->c == 257) {
    //         // std::cout << "found ESC" << std::endl;
    //         return stream.getChar();
    //     } else {
    //         return current_node->c;
    //     }
    // }

    HuffmanCodec::HuffmanCodec() {
        tree = new HuffmanTree();
    }

    void HuffmanCodec::learn(const vector<string>& vec) {
        clock_t begin = clock();
        for (const string& str : vec) {
            tree->LearnOnString(str);
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "learning done in " << elapsed_secs << " secs" << std::endl;
        tree->Build();
    }

    void HuffmanCodec::encode(const string_view& raw, string& encoded) const {
        encoded.reserve(2 * raw.size());
        // char* encoded = new char[2 * raw.size()];
        // char* ptr = encoded;
        uint8_t bitContainer = 0;
        size_t bitOffset = 0;
        for (size_t i = 0; i < raw.size() + 1; i++) {
            unsigned int ch;
            if (i != raw.size()) {
                ch = raw[i];
            } else {
                ch = 256;
            }
            Codeword* cw = tree->char_table[bitOffset][ch];
            // std::cout << "merging codeword " << ch << " = " << *tree->char_table[0][ch] << ", offset is " << bitOffset << std::endl;
            bitContainer |= cw->packedBits[0];
            if (cw->size > 1) {
                // *ptr = bitContainer;
                // ptr++;
                encoded.push_back(bitContainer);
                for (size_t j = 1; j < cw->size - 1; j++) {
                    encoded.push_back(cw->packedBits[i]);
                }
                // memcpy(ptr, cw->packedBits, (cw->size - 1) * sizeof(uint8_t));
                // ptr += cw->size - 2;
                bitContainer = cw->packedBits[cw->size - 1];
            }
            bitOffset = cw->lastBitsCount;
        }
        if (bitOffset > 0) {
            // *ptr = bitContainer;
            // ptr++;
            encoded.push_back(bitContainer);
        }

        // for (char c : encoded) {
        //     int bitPos = 7;
        //     while (bitPos >= 0) {
        //         bool bit = c & (1 << bitPos);
        //         std::cout << bit;
        //         bitPos--;
        //     }
        // }
        // std::cout << std::endl;
        // return string_view(encoded, ptr - encoded);
    }

    void HuffmanCodec::decode(const string_view& raw, string& result) const {
        result.reserve(2 * raw.size());
        int bitPos = 7;
        size_t pos = 0;
        uint8_t bitContainer = raw[0];
        HuffmanNode* ptr = tree->root;
        bool decoding = true;
        while (decoding) {
            while (!(ptr->left == nullptr && ptr->right == nullptr)) {
                if (bitPos == -1) {
                    pos++;
                    bitContainer = raw[pos];
                    bitPos = 7;
                }
                bool bit = bitContainer & (1 << bitPos);
                bitPos--;
                // std::cout << bit;
                if (bit) {
                    ptr = ptr->right;
                } else {
                    ptr = ptr->left;
                }
            }
            if (ptr->c == 256) {
                decoding = false;
            } else {
                result.push_back(ptr->c);
            }
            ptr = tree->root;
        }
        // std::cout << std::endl;
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
