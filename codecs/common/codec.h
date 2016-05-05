#pragma once

#include <experimental/algorithm>
#include <experimental/string_view>

#include <exception>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

namespace Codecs {

    using std::string;
    using std::experimental::string_view;
    using std::vector;


    class CodecException : public std::exception {
        std::string descr;
    public:
        CodecException() = default;
        CodecException(CodecException&& c) noexcept {
            (*this) = std::move(c);
        }

        CodecException& operator=(CodecException&& c) noexcept {
            descr = std::move(c.descr);
            (std::exception&)(*this) = std::move(c);
            return *this;
        }

        explicit CodecException(const std::string& d)
            : descr(d)
        {}

        const char* what() const noexcept override {
            return descr.c_str();
        }
    };

#if defined(cthrow)
    #error "already defined"
#else
    #define cthrow(what) do { std::ostringstream s; s << "at " << __FILE__ << ":" << __LINE__ << " " << what; \
        throw CodecException(s.str()); \
    } while (false)
#endif

    class CodecIFace {
    public:
        virtual void encode(const string_view&, std::string&) const = 0;
        virtual void decode(const string_view&, std::string&) const = 0;

        virtual string save() const = 0;
        virtual void load(const string_view&) = 0;

        virtual size_t sample_size(size_t records_total) const = 0;
        
        virtual void learn(const std::vector<std::string>& all_samples) = 0;

        virtual void reset() = 0;

        virtual ~CodecIFace() {}
    };

}