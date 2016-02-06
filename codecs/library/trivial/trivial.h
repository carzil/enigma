#pragma once

#include <library/common/codec.h>

namespace Codecs {

class TrivialCodec : public CodecIFace {
public:
    void encode(string& encoded, const string_view& raw) const override {
        encoded.assign(raw.begin(), raw.end());
    }

    void decode(string& raw, const string_view& encoded) const override {
        raw.assign(encoded.begin(), encoded.end());
    }

    string save() const override {
        return string();
    }

    void load(const string_view&) override {}

    size_t sample_size(size_t) const override {
        return 0;
    };

    void learn(const StringViewVector&) override {}

    void reset() override {}
};

}
