#ifndef SEQUENCE_VARIANT_MATRIX_INTERNAL_HPP__
#define SEQUENCE_VARIANT_MATRIX_INTERNAL_HPP__

#include <cstdint>
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

        template <typename T> struct col_view_
        {
            static_assert(std::is_pointer<T>::value, "T must be pointer type");
            T data;
            using dtype = typename std::remove_pointer<T>::type;
            std::size_t col_end, stride;

            col_view_(T data_, std::size_t col_end_, std::size_t stride_)
                : data(data_), col_end(col_end_), stride(stride_)
            {
            }
            inline dtype& operator[](const std::size_t i)
            {
                return data[i * stride];
            }
            inline const dtype& operator[](const std::size_t i) const
            {
                return data[i * stride];
            }
            inline dtype&
            at(const std::size_t i)
            {
                if (i >= col_end / stride)
                    {
                        throw std::out_of_range("index out of range");
                    }
                return data[i * stride];
            }
            inline const dtype&
            at(const std::size_t i) const
            {
                if (i >= col_end / stride)
                    {
                        throw std::out_of_range("index out of range");
                    }
                return data[i * stride];
            }
            std::size_t
            size() const
            {
                return col_end / stride;
            }

            template <typename POINTER> struct iterator_
            {
                using difference_type = std::int32_t;
                using value_type = dtype;
                mutable POINTER data;
                difference_type stride, offset;
                explicit iterator_(T data_, difference_type stride_)
                    : data{ data_ }, stride{ stride_ }, offset{ 0 }
                {
                }
                value_type& operator*() { return *(data + offset); }
                const value_type& operator*() const
                {
                    return *(data + offset);
                }

                iterator_&
                operator=(const iterator_& rhs)
                {
                    this->data = rhs.data;
                    this->stride = rhs.stride;
                    this->offset = rhs.offset;
                }
                iterator_&
                operator+(difference_type d)
                {
                    offset += d * stride;
                    return *this;
                }
                iterator_& operator++() { return *this + 1; }
                iterator_&
                operator+=(difference_type d)
                {
                    return *this + d;
                }

                iterator_&
                operator-(difference_type d)
                {
                    return *this + -d;
                }
                iterator_& operator--() { return *this - 1; }
                iterator_&
                operator-=(difference_type d)
                {
                    return *this - d;
                }
                bool
                operator<=(const iterator_ rhs) const
                {
                    return (this->data + this->offset)
                           <= (rhs.data + rhs.offset);
                }
                bool
                operator<(const iterator_ rhs) const
                {
                    return (this->data + this->offset)
                           < (rhs.data + rhs.offset);
                }
                bool
                operator>(const iterator_ rhs) const
                {
                    return !(*this <= rhs);
                }
                bool
                operator>=(const iterator_ rhs) const
                {
                    return !(*this < rhs);
                }
                bool
                operator==(const iterator_ rhs) const
                {
                    return !(*this < rhs) && !(*this > rhs);
                }
                bool
                operator!=(const iterator_ rhs) const
                {
                    return !(*this == rhs);
                }
            };

            using iterator = iterator_<T>;
            using const_iterator = iterator_<const T>;
            // using const_iterator = const iterator;

            iterator
            begin()
            {
                return iterator(data, stride);
            }
            iterator
            end()
            {
                return iterator(data + col_end, stride);
            }
            const_iterator
            begin() const
            {
                return const_iterator(data, stride);
            }
            const_iterator
            end() const
            {
                return const_iterator(data + col_end, stride);
            }
            const_iterator
            cbegin() const
            {
                return const_iterator(data, stride);
            }
            const_iterator
            cend() const
            {
                return const_iterator(data + col_end, stride);
            }
        };
    }
}

#endif
