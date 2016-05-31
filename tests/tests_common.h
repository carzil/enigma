#pragma once

#include "enigma/common/codec.h"
#include <gtest/gtest.h>

namespace Codecs {

void test_ed_string(CodecIFace& codec, const std::string& test_string) {
    codec.reset();
    std::string encoded, decoded;
    std::experimental::string_view v(test_string);
    codec.Learn(std::vector<std::string>({test_string}));
    codec.EndLearning();
    codec.encode(v, encoded);
    codec.decode(encoded, decoded);
    ASSERT_EQ(test_string, decoded);
}

}