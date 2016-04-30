#pragma once

namespace Codecs {

    class bitwriter {
        protected:
            int char_pos;
            unsigned char current_char;

            void fixCharPosition();
            virtual void appendChar(char c) = 0;

        public:
            bitwriter();
            bitwriter(unsigned char, int);

            void writeBit(bool bit);
            void writeBits(unsigned char bits, int nbits = 8);
            virtual void flush();
    };

    class bitreader {
        protected:
            int char_pos;
            unsigned char current_char;

        public:
            bitreader();

            bool readBit();
            int readBits(unsigned char &bits, int nbits = 8);
    };
}