#pragma once

namespace Codecs {

    class bitwriter {
        protected:
            unsigned char current_char;
            int char_pos;

            void fixCharPosition();
            virtual void appendChar(char c) = 0;

        public:
            bitwriter();
            bitwriter(unsigned char, int);

            int writeBit(bool bit);
            int writeBits(unsigned char bits, int nbits = 8);
            virtual void flush();
    };

    class bitreader {
        protected:
            unsigned char current_char;
            int char_pos;

        public:
            bitreader();

            bool readBit();
            int readBits(unsigned char &bits, int nbits = 8);
    };
}