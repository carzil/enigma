#include "enigma/common/dictionary.h"
#include <algorithm>
#include <iostream>

namespace Codecs {

Dictionary::Dictionary() {
    nodes = new Node[MAX_SIZE + 2];
    next_free = 2;
}

Dictionary::~Dictionary() {
    if (nodes) {
        delete[] nodes;
    }
}

const Dictionary::Node& Dictionary::GetNode(int ptr) const {
    return nodes[ptr];
}

}
