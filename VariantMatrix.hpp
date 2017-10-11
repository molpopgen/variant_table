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
        VariantMatrix(data_input&& data_, positions_input&& positions_)
            : data(std::forward<data_input>(data_)),
              positions(std::forward<positions_input>(positions_)),
              nsites(positions.size()), nsam(data.size() / positions.size())
        {
            if (data.size() % positions.size() != 0.0)
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

    template <typename T> struct _row_view
    {
        static_assert(std::is_pointer<T>::value, "T must be pointer type");
        T data;
        using dtype = typename std::remove_pointer<T>::type;
        std::size_t row_size;

        _row_view(T data_, std::size_t row_size_)
            : data(data_), row_size(row_size_)
        {
        }
        inline dtype& operator[](const std::size_t i) { return data[i]; }
        inline const dtype& operator[](const std::size_t i) const
        {
            return data[i];
        }
        inline dtype&
        at(const std::size_t i)
        {
            if (i >= row_size)
                {
                    throw std::out_of_range("index out of range");
                }
            return data[i];
        }
        inline const dtype&
        at(const std::size_t i) const
        {
            if (i >= row_size)
                {
                    throw std::out_of_range("index out of range");
                }
            return data[i];
        }
        std::size_t
        size() const
        {
            return row_size;
        }

        dtype*
        begin()
        {
            return data;
        }
        dtype*
        end()
        {
            return data + row_size;
        }
        const dtype*
        begin() const
        {
            return data;
        }
        const dtype*
        end() const
        {
            return data + row_size;
        }
        const dtype*
        cbegin() const
        {
            return data;
        }
        const dtype*
        cend() const
        {
            return data + row_size;
        }
    };

    using RowView = _row_view<std::int8_t*>;
    using ConstRowView = _row_view<const std::int8_t*>;

    // Rather than have member functions, we will have standalone functions:

    // The following could (should?) be declared noexcept(false):
    ConstRowView get_RowView(const VariantMatrix& m, const std::size_t row);

    RowView get_RowView(VariantMatrix& m, const std::size_t row);

    ConstRowView get_ConstRowView(const VariantMatrix& m,
                                  const std::size_t row);

    ConstRowView get_ConstRowView(VariantMatrix& m, const std::size_t row);
}

#endif
