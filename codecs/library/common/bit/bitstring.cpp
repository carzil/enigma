#include <iostream>
#include <string>
#include "bitstring.h"

namespace Codecs {
    // TODO: we can optimize this like bitstream

    Bitstring::Bitstring() {
        char_pos = 7;
        current_char = 0;
        string_pos = 0;
    }

    Bitstring::Bitstring(std::string* str) {
        this->str = str;
        current_char = 0;
        char_pos = 7;
        string_pos = 0;
    }

    Bitstring::Bitstring(std::string* str, unsigned char current_char, int char_pos, int string_pos) :
        bitwriter(current_char, char_pos), str(str), string_pos(string_pos) {}

    void Bitstring::revert() {
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

    void Bitstring::reset() {
        current_char = 0;
        string_pos = 0;
        str->clear();
    }

    void inline Bitstring::appendChar(char c) {
        str->push_back(c);
    }

    std::string char_to_bits(char x, int pos = -1) {
        std::string s;
        for (int i = 7; i > pos; i--) {
            s.push_back('0' + ((x >> i) & 1));
        }
        return s;
    }

    void Bitstring::write(obitstream& stream) {
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

    std::string Bitstring::format() {
        std::string s;
        for (size_t i = 0; i < str->size(); i++) {
            s += char_to_bits(str->at(i));
        }
        s += char_to_bits(current_char, char_pos);
        return s;
    }

    void Bitstring::merge(Bitstring other) {
        for (size_t i = 0; i < other.str->size(); i++) {
            writeBits(other.str->at(i));
        }
        writeBits(other.current_char, 7 - other.char_pos);
    }

    Bitstring* Bitstring::clone() {
        std::string* new_str = new std::string();
        new_str->assign(str->begin(), str->end());
        return new Bitstring(new_str, current_char, char_pos, string_pos);
    }

    size_t Bitstring::sizeInBits() {
        return 8 * str->size() + 7 - char_pos;
    }
}
