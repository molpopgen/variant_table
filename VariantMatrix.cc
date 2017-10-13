#include "VariantMatrix.hpp"
#include <iostream>

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
		std::cout << "check: " << data.size() << ' ' << nsam << ' ' << haplotype << ' ' << site <<  ' ' << haplotype*nsam + site << '\n';
        return data.at(haplotype * nsam + site);
    }

    const std::int8_t&
    VariantMatrix::at(const std::size_t site,
                      const std::size_t haplotype) const
    {
        return data.at(haplotype * nsam + site);
    }
}
