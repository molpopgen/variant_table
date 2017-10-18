#ifndef SEQUENCE_VARIANTMATRIX_HAPLOTYPE_COUNTS_HPP__
#define SEQUENCE_VARIANTMATRIX_HAPLOTYPE_COUNTS_HPP__

#include <utility>
#include <vector>
#include <cstdint>
#include <cstddef>
#include "VariantMatrixViews.hpp"

namespace Sequence
{
    bool haplotypes_differ(const ConstColView& row_i,
                           const ConstColView& row_j);

    std::vector<std::pair<std::size_t, std::uint32_t>>
    unique_haplotype_counts(const VariantMatrix& m);
}

#endif
