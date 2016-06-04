#pragma once
#include <experimental/string_view>

using std::experimental::string_view;

namespace Enigma {

void print_char_bits(char c);

class DataInput {
    public:
        DataInput(const std::string& s) : raw(s), pos(0) {}

        uint8_t ReadChar() {
            uint8_t r = raw[pos];
            pos++;
            return r;
        }

        int ReadInt() {
            int n = static_cast<uint8_t>(raw[pos]);
            n |= static_cast<uint8_t>(raw[pos + 1]) << 8;
            n |= static_cast<uint8_t>(raw[pos + 2]) << 16;
            n |= static_cast<uint8_t>(raw[pos + 3]) << 24;
            pos += 4;
            return n;
        }

    private:
        const std::string& raw;
        size_t pos;
};

class DataOutput {
    public:
        DataOutput() {}

        void WriteInt(int n) {
            raw.push_back(static_cast<uint8_t>(n));
            raw.push_back(static_cast<uint8_t>(n >> 8));
            raw.push_back(static_cast<uint8_t>(n >> 16));
            raw.push_back(static_cast<uint8_t>(n >> 24));
        }

        void WriteChar(uint8_t ch) {
            raw.push_back(static_cast<uint8_t>(ch));
        }

        const std::string& GetStr() {
            return raw;
        }

    private:
        std::string raw;
};

}