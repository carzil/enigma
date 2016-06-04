namespace Enigma {
    RLECodec::RLECodec() {

    }

    RLECodec::~RLECodec() {
        
    }

    void RLECodec::encode(bitstring& encoded, const string_view& raw) {
        char c_char = raw[0];
        int cnt = 1;
        for (size_t i = 1; i < raw.size(); i++) {
            if (raw[i] != c_char) {
                encoded.writeInt(cnt);
                encoded.writeBits(c_char, 8);
                c_char = raw[i];
                cnt = 1;
            } else {
                cnt++;
            }
        }
    }

    void RLECodec::decode(string& raw, bitstring& encoded) {

    }

    string RLECodec::save() {

    }

    void RLECodec::load(const string_view&) {

    }

    size_t RLECodec::sample_size(size_t records_total) {

    }

    void RLECodec::learn(const StringViewVector& all_samples) {

    }

    void RLECodec::reset() {

    }
}