#ifndef SEQUENCE_VARIANTMATRIX_STATECOUNTS_HPP__
#define SEQUENCE_VARIANTMATRIX_STATECOUNTS_HPP__

#include "VariantMatrixViews.hpp"
#include <unordered_map>
#include <vector>

namespace Sequence
{
    struct StateCounts
    {
        std::unordered_map<std::int8_t, std::uint32_t> counts;
        std::int8_t refstate;
        StateCounts(const ConstRowView& r,
                    const std::int8_t refstate_ = -1);
    };

    std::vector<StateCounts>
    process_variable_sites(const VariantMatrix& m,
                           const std::vector<std::int8_t>& refstates = std::vector<int8_t>());
}

#endif
