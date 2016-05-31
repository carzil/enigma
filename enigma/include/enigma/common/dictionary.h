#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

#include "enigma_export.h"

namespace Codecs {


class ENIGMA_NO_EXPORT Dictionary {
    public:
        static const size_t MAX_SIZE = 1 << 17;

        struct Node {
            unsigned char ch;
            int* table;
            
            int parent;
            size_t depth;

            Node() : parent(1), depth(0), ch(0), table(nullptr) {}
            ~Node() {
                if (table) {
                    delete[] table;
                }
            }

        };

        Dictionary();
        ~Dictionary();

        int AddNode(int ptr, unsigned char c) {
            Node* node = nodes + ptr;
            if (!node->table) {
                node->table = new int[256];
                memset(node->table, 0, sizeof(int) * 256);
            }
            if (node->table[c] == 0) {
                node->table[c] = CreateNode(ptr, c);
            }
            return node->table[c];
        }

        int CreateNode(int parent, unsigned char ch) {
            Node* node = nodes + next_free;
            node->parent = parent;
            node->ch = ch;
            node->depth = nodes[parent].depth + 1;
            size_t res = next_free;
            next_free++;
            return res;
        }

        int NextNode(int ptr, unsigned char c) const {
            Node* node = nodes + ptr;
            return (node->table) ? node->table[c] : 0;
        }

        size_t Size() const {
            return next_free;
        }

        bool IsFull() const {
            return Size() >= MAX_SIZE;
        }

        bool IsValid(int ptr) const {
            return ptr > 0;
        }

        int GetRoot() const {
            return 1;
        }

        const Dictionary::Node& GetNode(int ptr) const;
        std::string RestoreString(int ptr) const {
            std::string s;
            while (nodes[ptr].parent != 1) {
                s.push_back(nodes[ptr].ch);
                ptr = nodes[ptr].parent;
            }
            s.push_back(nodes[ptr].ch);
            std::reverse(s.begin(), s.end());
            return s;
        }

    private:
        Node* nodes;
        size_t next_free;
};

}
