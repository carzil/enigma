#pragma once

#include <experimental/algorithm>
#include <experimental/string_view>
#include <random>
#include <vector>

namespace Codecs {
    using std::experimental::string_view;
    using std::vector;
    using StringViewVector = vector<string_view>;

    template <typename Iter>
    static void select_sample(StringViewVector& sample, Iter begin, Iter end, size_t sample_size) {
        std::experimental::sample(begin, end, std::back_inserter(sample), sample_size, std::mt19937());
    }
}
