#include <iostream>
#include <string>
#include "bitstring.h"

namespace Codecs {
    bitstring::bitstring() {
        this->str = new std::string();
        char_pos = 7;
        current_char = 0;
        string_pos = 0;
    }

    bitstring::bitstring(std::string* str) {
        this->str = str;
        current_char = 0;
        char_pos = 7;
        if (str->size() > 0) {
            current_char = str->at(0);
        }
        string_pos = 0;
    }

    bitstring::bitstring(std::string* str, unsigned char current_char, int char_pos, int string_pos) :
        bitwriter(current_char, char_pos), str(str), string_pos(string_pos) {}

    void bitstring::revert() {
        char_pos++;
        if (char_pos == 7) {
            if (str->size() > 0) {
                current_char = str->back();
                str->resize(str->size() - 1);
                char_pos = -1;
            } else {
                current_char = 0;
                char_pos = 7;
            }
        }
    }

    void bitstring::reset() {
        current_char = 0;
        string_pos = 0;
        str->clear();
    }

    void inline bitstring::appendChar(char c) {
        str->push_back(c);
    }

    std::string char_to_bits(char x, int pos = -1) {
        std::string s;
        for (int i = 7; i > pos; i--) {
            s.push_back('0' + ((x >> i) & 1));
        }
        return s;
    }

    bool bitstring::getBit() {
        if (char_pos == -1) {
            string_pos++;
            if (string_pos == str->size()) {
                throw std::out_of_range("no more chars provided");
            } else {
                current_char = str->at(string_pos);
                char_pos = 7;
            }
        }
        bool bit = current_char & (1 << char_pos);
        char_pos--;
        return bit;
    }

    void bitstring::write(obitstream& stream) {
        // std::cout << "writing: " << format() << std::endl;
        for (size_t i = 0; i < str->size(); i++) {
            unsigned char w = str->at(i);
            // std::cout << "here: " << (int)(unsigned char)(BIT_REVERSE[w]) << " (" << (int)(unsigned char)str->at(i) << ")" << std::endl;
            stream.writeBits(BIT_REVERSE[w], 8);
        }
        // std::cout << "c_c = " << (int)(unsigned char)current_char << ", r = " << (int)(unsigned char)BIT_REVERSE[current_char] << ", char_pos = " << char_pos << std::endl;
        // std::cout << "====" << std::endl;
        stream.writeBits(BIT_REVERSE[current_char], 7 - char_pos);
    }

    std::string bitstring::format() {
        std::string s;
        for (size_t i = 0; i < str->size(); i++) {
            s += char_to_bits(str->at(i));
        }
        s += char_to_bits(current_char, char_pos);
        return s;
    }

    void bitstring::merge(bitstring other) {
        for (size_t i = 0; i < other.str->size(); i++) {
            writeBits(other.str->at(i));
        }
        writeBits(other.current_char, 7 - other.char_pos);
    }

    bitstring* bitstring::clone() {
        std::string* new_str = new std::string();
        new_str->assign(str->begin(), str->end());
        return new bitstring(new_str, current_char, char_pos, string_pos);
    }

    size_t bitstring::sizeInBits() {
        return 8 * str->size() + 7 - char_pos;
    }
}
