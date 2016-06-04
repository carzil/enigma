#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>

#include "enigma_export.h"
#include "enigma/utils.h"

namespace Enigma {

class ENIGMA_NO_EXPORT Dictionary {
    public:
        static const size_t MAX_SIZE = 1 << 17;

        struct Node {

            uint8_t ch;

            uint16_t size;
            uint16_t capacity;

            uint8_t* chars;
            int* nexts;
            
            int parent;
            size_t depth;

            Node() : ch(0), size(0), capacity(4), chars(nullptr), nexts(nullptr), parent(1), depth(0) {
                chars = new uint8_t[4];
                nexts = new int[4];
                memset(chars, 0, sizeof(uint8_t) * 4);
                memset(nexts, 0, sizeof(int) * 4);
            }

            ~Node() {
                if (chars) {
                    delete[] chars;
                }

                if (nexts) {
                    delete[] nexts;
                }
            }

        };

        Dictionary();
        ~Dictionary();

        int AddNode(int ptr, uint8_t c) {
            Node* node = nodes + ptr;

            if (node->capacity == 256) {
                if (node->nexts[c] == 0) {
                    node->nexts[c] = CreateNode(ptr, c);
                }
                return node->nexts[c];
            } else {
                for (size_t i = 0; i < node->size; i++) {
                    if (node->chars[i] == c) {
                        return node->nexts[i];
                    }
                }
                // uint8_t* p = (uint8_t*)memchr(node->chars, c, node->size);
                // if (p) {
                //     return node->nexts[p - node->chars];
                // }
                if (node->size >= node->capacity) {
                    if (node->capacity <= 16) {
                        uint8_t* new_chars = new uint8_t[node->capacity * 2];
                        int* new_nexts = new int[node->capacity * 2];
                        memset(new_chars, 0, sizeof(uint8_t) * node->capacity * 2);
                        memset(new_nexts, 0, sizeof(int) * node->capacity * 2);
                        memcpy(new_chars, node->chars, sizeof(uint8_t) * node->size);
                        memcpy(new_nexts, node->nexts, sizeof(int) * node->size);
                        delete[] node->chars;
                        delete[] node->nexts;
                        node->chars = new_chars;
                        node->nexts = new_nexts;
                        node->capacity *= 2;
                    } else {
                        int* new_nexts = new int[256];
                        memset(new_nexts, 0, sizeof(int) * 256);
                        for (size_t i = 0; i < node->size; i++) {
                            new_nexts[node->chars[i]] = node->nexts[i];
                        }
                        delete[] node->chars;
                        delete[] node->nexts;
                        node->chars = nullptr;
                        node->nexts = new_nexts;
                        node->capacity = 256;
                        node->size = 256;
                        node->nexts[c] = CreateNode(ptr, c);
                        return node->nexts[c];
                    }
                }
                int n = CreateNode(ptr, c);
                node->chars[node->size] = c;
                node->nexts[node->size] = n;
                node->size++;
                return n;
            }
        }

        int CreateNode(int parent, uint8_t ch) {
            Node* node = nodes + next_free;
            node->parent = parent;
            node->ch = ch;
            node->depth = nodes[parent].depth + 1;
            size_t res = next_free;
            next_free++;
            return res;
        }

        int NextNode(int ptr, uint8_t c) const {
            Node* node = nodes + ptr;
            if (node->capacity == 256) {
                return node->nexts[c];
            } else {
                // for (size_t i = 0; i < node->size; i++) {
                //     std::cout << (int)(uint8_t)node->chars[i] << " "; 
                // }
                // std::cout << std::endl;

                // for (size_t i = 0; i < node->size; i++) {
                //     std::cout << node->nexts[i] << " "; 
                // }
                // std::cout << std::endl;
                for (size_t i = 0; i < node->size; i++) {
                    if (node->chars[i] == c) {
                        return node->nexts[i];
                    }
                }
                return 0;
                // uint8_t* p = (uint8_t*)memchr(node->chars, c, node->size);
                // if (!p) {
                //     return 0;
                // } else {
                //     // std::cout << "found '" << c << "' at pos = " << p - node->chars << std::endl;
                //     // std::cout << "next = " << node->nexts[p - node->chars] << std::endl;
                //     return node->nexts[p - node->chars];
                // }
            }
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
