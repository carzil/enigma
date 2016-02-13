#pragma once

#include <library/common/codec.h>

namespace Codecs {

    class ZlibNoDictCodec : public CodecIFace {
    public:
        void encode(string& encoded, const string_view& raw) const override;
        void decode(string& raw, const string_view& encoded) const override;

        string save() const override {
            return string();
        }

        void load(const string_view&) override {}

        size_t sample_size(size_t) const override {
            return 0;
        }

        void learn(const StringViewVector&) {
        }

        void reset() {}
    };

}
