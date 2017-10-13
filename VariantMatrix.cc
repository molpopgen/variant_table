#include "VariantMatrix.hpp"
#include <iostream>

namespace Sequence
{
    // Non range-checked access
    std::int8_t&
    VariantMatrix::get(const std::size_t site, const std::size_t haplotype)
    {
        return data[site * nsam + haplotype];
    }

    const std::int8_t&
    VariantMatrix::get(const std::size_t site,
                       const std::size_t haplotype) const
    {
        return data[site * nsam + haplotype];
    }

    // Ranged-checked access after std::vector<T>::at.
    std::int8_t&
    VariantMatrix::at(const std::size_t site, const std::size_t haplotype)
    {
        return data.at(site * nsam + haplotype);
    }

    const std::int8_t&
    VariantMatrix::at(const std::size_t site,
                      const std::size_t haplotype) const
    {
        return data.at(site * nsam + haplotype);
    }
}
