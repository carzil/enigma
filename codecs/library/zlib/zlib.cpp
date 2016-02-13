#include "zlib.h"

#include <external/zlib/zlib.h>

namespace Codecs {

    void ZlibNoDictCodec::encode(string& encoded, const string_view& raw) const {
        encoded.clear();
        size_t raw_size = raw.size();

        while (raw_size) {
            encoded.push_back((char)((raw_size > 0x7F ? 0x80 : 0) | (raw_size & 0x7F)));
            raw_size >>= 7;
        }

        size_t res_size_max = compressBound(raw.size());
        size_t old_res_size = encoded.size();
        encoded.resize(old_res_size + res_size_max);
        uLongf destLen = 0;

        int res = compress((Bytef*)&*(encoded.begin() + old_res_size), &destLen, (const Bytef *) &*raw.begin(), raw.size());

        if (Z_OK != res) {
            cthrow("badly encoded: res=" << res);
        }

        encoded.resize(destLen + old_res_size);
    }

    void ZlibNoDictCodec::decode(string& raw, const string_view& encoded) const {
        raw.clear();
        size_t res_size_max = 0;
        const char* enc_beg = encoded.cbegin();
        const char* enc_end = encoded.cend();
        while (enc_beg < enc_end) {
            res_size_max <<= 7;
            char c = *enc_beg;
            res_size_max |= (c & 0x7F);
            enc_beg++;
            if (!(c & 0x80)) {
                break;
            }
        }

        raw.resize(res_size_max);
        uLongf destLen = 0;

        if (Z_OK != uncompress((Bytef*)&*(raw.begin()), &destLen,
                               (const Bytef*)enc_beg, enc_end - enc_beg) || destLen != res_size_max) {
            throw CodecException("badly decoded");
        }
    }
}
