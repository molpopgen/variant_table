#ifndef SEQUENCE_VARIANT_MATRIX_FILTERING_HPP_
#define SEQUENCE_VARIANT_MATRIX_FILTERING_HPP_

#include "VariantMatrix.hpp"
#include "VariantMatrixViews.hpp"
#include <functional>
#include <cstdint>

namespace Sequence
{
    std::int32_t
    filter_sites(VariantMatrix &m,
                 const std::function<bool(const RowView &)> &f);
}

#endif
