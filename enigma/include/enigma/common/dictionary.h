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

            uint8_t* chars;
            int* nexts;
            
            int parent;
            size_t depth;

            Node() : ch(0), chars(nullptr), nexts(nullptr), parent(1), depth(0) {
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

        Dictionary() {
            nodes = new Node[MAX_SIZE + 2];
            capacities = new uint8_t[MAX_SIZE + 2];
            sizes = new uint8_t[MAX_SIZE + 2];
            for (size_t i = 0; i < MAX_SIZE + 2; i++) {
                capacities[i] = 4;
                sizes[i] = 0;
            }
            // memset(capacities, 4, sizeof(uint8_t) * (MAX_SIZE + 2));
            next_free = 2;
            root = 1;
        }

        ~Dictionary() {
            delete[] nodes;
            delete[] capacities;
            delete[] sizes;
        }

        int AddNode(int ptr, uint8_t c) {
            // std::cout << (int*)capacities << std::endl;
            // for (size_t i = 0; i < Size(); i++) {
            //     std::cout << (int)capacities[i] << " ";
            // }
            // std::cout << std::endl;
            Node* node = nodes + ptr;
            uint8_t capacity = *(capacities + ptr);
            uint8_t size = *(sizes + ptr);

            // std::cout << ptr << " " << (int)c << " " << (int)capacity << std::endl;
            // std::cout << node->nexts << std::endl;

            if (capacity == 255) {
                if (node->nexts[c] == 0) {
                    node->nexts[c] = CreateNode(ptr, c);
                }
                return node->nexts[c];
            } else {
                for (size_t i = 0; i < size; i++) {
                    if (node->chars[i] == c) {
                        return node->nexts[i];
                    }
                }
                // uint8_t* p = (uint8_t*)memchr(node->chars, c, node->size);
                // if (p) {
                //     return node->nexts[p - node->chars];
                // }
                if (size >= capacity) {
                    // std::cout << (int)capacities[ptr] << std::endl;
                    if (capacity <= 16) {
                        uint8_t* new_chars = new uint8_t[capacity * 2];
                        int* new_nexts = new int[capacity * 2];
                        memset(new_chars, 0, sizeof(uint8_t) * capacity * 2);
                        memset(new_nexts, 0, sizeof(int) * capacity * 2);
                        memcpy(new_chars, node->chars, sizeof(uint8_t) * size);
                        memcpy(new_nexts, node->nexts, sizeof(int) * size);
                        delete[] node->chars;
                        delete[] node->nexts;
                        node->chars = new_chars;
                        node->nexts = new_nexts;
                        // std::cout << "reallocation for node " << ptr << ", last capacity = " << (int)capacity << std::endl;
                        capacities[ptr] *= 2;
                    } else {
                        int* new_nexts = new int[256];
                        memset(new_nexts, 0, sizeof(int) * 256);
                        for (size_t i = 0; i < size; i++) {
                            new_nexts[node->chars[i]] = node->nexts[i];
                        }
                        delete[] node->chars;
                        delete[] node->nexts;
                        node->chars = nullptr;
                        node->nexts = new_nexts;
                        capacities[ptr] = 255;
                        sizes[ptr] = 255;
                        node->nexts[c] = CreateNode(ptr, c);
                        // std::cout << "full reallocation for node " << ptr << ", c = " << (int)c << ", nexts = " << node->nexts << std::endl;
                        return node->nexts[c];
                    }
                }
                int n = CreateNode(ptr, c);
                node->chars[size] = c;
                node->nexts[size] = n;
                sizes[ptr]++;
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
            // for (size_t i = 0; i < Size(); i++) {
            //     std::cout << (int)capacities[i] << " ";
            // }
            // std::cout << std::endl;
            Node* node = nodes + ptr;
            uint8_t size = sizes[ptr];
            if (capacities[ptr] == 255) {
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
                for (uint8_t i = 0; i < size; i++) {
                    if (node->chars[i] == c) {
                        return node->nexts[i];
                    }
                }
                return 0;
                //     // std::cout << "found '" << c << "' at pos = " << p - node->chars << std::endl;
                //     // std::cout << "next = " << node->nexts[p - node->chars] << std::endl;
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
            return root;
        }

        const Dictionary::Node& GetNode(int ptr) const {
            return nodes[ptr];
        }

        std::string RestoreString(int ptr) const {
            std::string s;
            while (nodes[ptr].parent != root) {
                s.push_back(nodes[ptr].ch);
                ptr = nodes[ptr].parent;
                // std::cout << ptr << std::endl;
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

            for (size_t i = 0; i < Dictionary::MAX_SIZE + 2; i++) {
                capacities[i] = 4;
            }

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

        void Reorder(size_t* frequencies, size_t* new_frequencies, size_t* old_frequencies) {
            std::vector<size_t> ns;

            for (size_t i = 0; i < Size(); i++) {
                ns.push_back(i);
            }

            cmp c(frequencies);

            std::sort(ns.begin(), ns.end(), c);

            Node* new_nodes = new Node[Dictionary::MAX_SIZE + 2];
            memset(new_nodes, 0, sizeof(Node) * (Dictionary::MAX_SIZE + 2));
            uint8_t* new_capacities = new uint8_t[Dictionary::MAX_SIZE + 2];
            uint8_t* new_sizes = new uint8_t[Dictionary::MAX_SIZE + 2];

            size_t* new_poses = new size_t[Dictionary::MAX_SIZE + 2];

            ns.insert(ns.begin(), 0);

            for (size_t i = 0; i < ns.size(); i++) {
                new_poses[ns[i]] = i;
                // std::cout << "remap " << ns[i] << " -> " << i << " (freq = " << frequencies[ns[i]] << ")" << std::endl;
            }

            for (size_t i = 0; i < ns.size(); i++) {
                size_t old_pos = ns[i];
                size_t new_pos = new_poses[old_pos];
                size_t parent_old_pos = nodes[old_pos].parent;
                size_t parent_new_pos = new_poses[parent_old_pos];

                new_nodes[new_pos].ch = nodes[old_pos].ch;
                new_nodes[new_pos].depth = nodes[old_pos].depth;
                new_nodes[new_pos].parent = new_poses[parent_old_pos];

                if (parent_old_pos == 0) {
                    new_nodes[new_pos].parent = 0;
                }
                // std::cout << parent_new_pos << " <-" << nodes[old_pos].ch << "-> " << new_pos << " " << new_nodes[new_pos].parent << std::endl;

                new_capacities[new_pos] = capacities[old_pos];
                new_sizes[new_pos] = sizes[old_pos];

                if (capacities[parent_old_pos] == 255) {
                    // std::cout << nodes[old_pos].ch << std::endl;
                    if (new_nodes[parent_new_pos].nexts == 0) {
                        new_nodes[parent_new_pos].nexts = new int[256];
                        new_nodes[parent_new_pos].chars = nullptr;
                        // std::cout << "here" << std::endl;
                    }
                    new_nodes[parent_new_pos].nexts[nodes[old_pos].ch] = new_pos;
                } else {
                    if (new_nodes[parent_new_pos].nexts == 0) {
                        new_nodes[parent_new_pos].chars = new uint8_t[capacities[parent_old_pos]];
                        new_nodes[parent_new_pos].nexts = new int[capacities[parent_old_pos]];
                        // std::cout << "and here" << std::endl;
                    }
                    for (size_t j = 0; j < sizes[parent_old_pos]; j++) {
                        if (nodes[parent_old_pos].chars[j] == nodes[old_pos].ch) {
                            new_nodes[parent_new_pos].chars[j] = nodes[old_pos].ch;
                            new_nodes[parent_new_pos].nexts[j] = new_pos;
                        }
                    }
                }
            }

            for (size_t i = 0; i < ns.size(); i++) {
                new_frequencies[i] = old_frequencies[ns[i]];
            }

            root = new_poses[root];
            delete[] new_poses;
            delete[] sizes;
            delete[] capacities;
            delete[] nodes;
            nodes = new_nodes;
            sizes = new_sizes;            
            capacities = new_capacities;

            // for (size_t i = 2; i < Size(); i++) {
            //     std::cout << RestoreString(i) << std::endl;
            // }
        }

    private:
        Node* nodes;
        uint8_t* capacities;
        uint8_t* sizes;
        size_t next_free;
        int root;

        class cmp {
            size_t* frequencies;

            public:
                cmp(size_t* frequencies) : frequencies(frequencies) {}

                bool operator()(size_t a, size_t b) {
                    return frequencies[a] > frequencies[b];
                }
        };
};

}
