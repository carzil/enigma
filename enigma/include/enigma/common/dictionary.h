#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <experimental/string_view>

#include "enigma_export.h"
#include "enigma/utils.h"

using std::experimental::string_view;

namespace Enigma {


class ENIGMA_NO_EXPORT Dictionary {
    public:
        static const size_t MAX_SIZE = 1 << 17;

        struct Node {
            unsigned char ch;
            int* table;
            
            int parent;
            size_t depth;

            Node() : ch(0), table(nullptr), parent(1), depth(0) {}
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

        void Save(DataOutput& out) const {
            out.WriteInt(Size());
            // std::cout << "size = " << Size() << std::endl;
            for (size_t i = 2; i < Size(); i++) {
                out.WriteChar(nodes[i].ch);
                out.WriteInt(nodes[i].parent);
                // std::cout << "saved " << i << " " << nodes[i].ch << " " << std::endl;
            }
        }

        void Load(DataInput& in) {
            delete[] nodes;
            nodes = new Node[Dictionary::MAX_SIZE + 2];

            next_free = 2;
            size_t size = in.ReadInt();
            // std::cout << "size = " << size << std::endl;
            for (size_t i = 2; i < size; i++) {
                uint8_t ch = in.ReadChar();
                int parent = in.ReadInt();
                AddNode(parent, ch);
                // std::cout << "added " << parent << " " << ch << std::endl;
            }
        }

    private:
        Node* nodes;
        size_t next_free;
};

}
