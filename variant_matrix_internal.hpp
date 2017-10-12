#ifndef SEQUENCE_VARIANT_MATRIX_INTERNAL_HPP__
#define SEQUENCE_VARIANT_MATRIX_INTERNAL_HPP__

#include <cstddef>
#include <stdexcept>
#include <type_traits>

namespace Sequence
{
    namespace internal
    {
        template <typename T> struct row_view_
        {
            static_assert(std::is_pointer<T>::value, "T must be pointer type");
            T data;
            using dtype = typename std::remove_pointer<T>::type;
            std::size_t row_size;

            row_view_(T data_, std::size_t row_size_)
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
    }
}

#endif
