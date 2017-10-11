#ifndef SEQUENCE_VARIANT_MATRIX_HPP__
#define SEQUENCE_VARIANT_MATRIX_HPP__

#include <cstdint>
#include <cstddef>
#include <utility>
#include <vector>
#include <stdexcept>
#include <type_traits>

static_assert(sizeof(std::int8_t) == sizeof(char),
              "sizeof(char) is not 8 bits");

namespace Sequence
{
    struct VariantMatrix
    ///
    /// Matrix representation of variation data.
    /// The data structure is a row-major matrix.
    /// Variants are represented by 8-bit integers.
    /// Negative values represent missing data,
    /// thus allowing up to 128 non-missing states
    /// per variable site.
    ///
    /// Rows are sites, columns are either haplotypes
    /// or are populated as necessary in the case of
    /// genotype (unphased) data.  Storing variable
    /// sites in rows places the performance priority
    /// on sites over haplotypes.
    ///
    /// The only reserved character state is
    /// std::numeric_limits<std::int8_t>::min(),
    /// which is used to represent masked data.
    ///
    /// \version 1.9.2
    {
        std::vector<std::int8_t> data;
        std::vector<double> positions;
        std::size_t nsites;
        std::size_t nsam;

        template <typename data_input, typename positions_input>
        VariantMatrix(data_input&& data_, positions_input&& positions_,
                      const std::size_t nvariants)
            : data(std::forward<data_input>(data_)),
              positions(std::forward<positions_input>(positions_)),
              nsites(nvariants), nsam(data.size() / invariants)
        {
            if (data.size() % nvariants != 0.0)
                {
                    throw std::invalid_argument("incorrect dimensions");
                }
        }

        // Non range-checked access
        std::int8_t& get(const std::size_t site, const std::size_t haplotype);
        const std::int8_t& get(const std::size_t site,
                               const std::size_t haplotype) const;

        // Ranged-checked access after std::vector<T>::at.
        std::int8_t& at(const std::size_t site, const std::size_t haplotype);
        const std::int8_t& at(const std::size_t site,
                              const std::size_t haplotype) const;
    };

    // Rather than have member functions, we will have standalone functions:
}

#endif
