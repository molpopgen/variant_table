#include "VariantMatrix.hpp"

namespace Sequence
{
    // Non range-checked access
    std::int8_t&
    VariantMatrix::get(const std::size_t site, const std::size_t haplotype)
    {
        return data[haplotype * nsam + site];
    }

    const std::int8_t&
    VariantMatrix::get(const std::size_t site,
                       const std::size_t haplotype) const
    {
        return data[haplotype * nsam + site];
    }

    // Ranged-checked access after std::vector<T>::at.
    std::int8_t&
    VariantMatrix::at(const std::size_t site, const std::size_t haplotype)
    {
        return data.at(haplotype * nsam + site);
    }

    const std::int8_t&
    VariantMatrix::at(const std::size_t site,
                      const std::size_t haplotype) const
    {
        return data.at(haplotype * nsam + site);
    }

    template <typename T, typename VM>
    T
    row_view_wrapper(VM& m, const std::size_t row)
    {
        if (row >= m.nsites)
            {
                throw std::out_of_range("row index out of range");
            }
        return T(m.data.data() + row * m.nsam, m.nsam);
    }

    ConstRowView
    get_RowView(const VariantMatrix& m, const std::size_t row)
    {
        return row_view_wrapper<ConstRowView>(m, row);
    }

    RowView
    get_RowView(VariantMatrix& m, const std::size_t row)
    {
        return row_view_wrapper<RowView>(m, row);
    }

    ConstRowView
    get_ConstRowView(const VariantMatrix& m, const std::size_t row)
    {
        return row_view_wrapper<ConstRowView>(m, row);
    }

    ConstRowView
    get_ConstRowView(VariantMatrix& m, const std::size_t row)
    {
        return row_view_wrapper<ConstRowView>(m, row);
    }
}
