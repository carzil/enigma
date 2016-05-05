#include "tests_common.h"

namespace Codecs {
    void test_ed_string(CodecIFace& codec, const std::string& test_string) {
        codec.reset();
        std::string encoded, decoded;
        std::experimental::string_view v(test_string);
        codec.learn(std::vector<std::string>({test_string}));
        codec.encode(v, encoded);
        codec.decode(encoded, decoded);
        ASSERT_EQ(test_string, decoded);
    }
}
