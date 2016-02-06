#include <library/zlib/zlib.h>
#include <library/tests_common/tests_common.h>

TEST(ZlibNoDictCodecTest, Works) {
    Codecs::ZlibNoDictCodec codec;
    Codecs::TestSimple(codec);
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
