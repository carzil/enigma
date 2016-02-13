#include "tests_common.h"

#include <external/gtest/gtest.h>

namespace Codecs {

    static const char* TEST_SIMPLE_DATA[] = {
            "aaaa", "bbbb", "cccc"
    };

    void test_simple(CodecIFace& c) {
        c.reset();
        auto b = std::begin(TEST_SIMPLE_DATA);
        auto e = std::end(TEST_SIMPLE_DATA);
        CodecIFace::train(c, b, e, e - b);
        std::string data[2];
        c.encode(data[0], TEST_SIMPLE_DATA[0]);
        c.decode(data[1], data[0]);
        GTEST_ASSERT_EQ(std::string(TEST_SIMPLE_DATA[0]), data[1]);
    }
}
