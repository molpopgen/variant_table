#ifndef SEQUENCE_VARIANT_MATRIX_VIEWS_HPP__
#define SEQUENCE_VARIANT_MATRIX_VIEWS_HPP__

#include "VariantMatrix.hpp"
#include "variant_matrix_views_internal.hpp"

namespace Sequence
{
    using RowView = internal::row_view_<std::int8_t*>;
    using ConstRowView = internal::row_view_<const std::int8_t*>;
    using ColView = internal::col_view_<std::int8_t*>;
    using ConstColView = internal::col_view_<const std::int8_t*>;

    // Rather than have member functions, we will have standalone functions:

    // The following could (should?) be declared noexcept(false):
    ConstRowView get_RowView(const VariantMatrix& m, const std::size_t row);

    RowView get_RowView(VariantMatrix& m, const std::size_t row);

    ConstRowView get_ConstRowView(const VariantMatrix& m,
                                  const std::size_t row);

    ConstRowView get_ConstRowView(VariantMatrix& m, const std::size_t row);

    ColView get_ColView(VariantMatrix& m, const std::size_t col);
    ConstColView get_ColView(const VariantMatrix& m, const std::size_t col);
    ConstColView get_ConstColView(VariantMatrix& m, const std::size_t col);
    ConstColView get_ConstColView(const VariantMatrix& m,
                                  const std::size_t col);
}

#endif
