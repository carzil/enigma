#include <library/trivial/trivial.h>
#include <library/tests_common/tests_common.h>

TEST(TrivialCodecTest, Works) {
    Codecs::TrivialCodec codec;
    Codecs::TestSimple(codec);
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
