#include <library/common/codec.h>

class RLECodec : public CodecIFace {
        public:
            RLECodec();
            ~RLECodec();

            virtual void encode(bitstring& encoded, const string_view& raw) const override;
            virtual void decode(string& raw, bitstring& encoded) const override;

            virtual string save() const override;
            virtual void load(const string_view&) override;

            virtual size_t sample_size(size_t records_total) const override;
            virtual void learn(const StringViewVector& all_samples) override;

            virtual void reset() override;
};