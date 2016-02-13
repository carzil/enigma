#include <library/trivial/trivial.h>
#include <library/tests_common/tests_common.h>

TEST(TrivialCodecTest, Works) {
    using namespace Codecs;
    TrivialCodec codec;
    test_simple(codec);
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
