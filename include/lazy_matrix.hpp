#pragma once

#include <istream>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <functional>
#include <iterator>
#include <concepts>
#include <memory>
#include <type_traits>
#include <initializer_list>
#include <utility>

#include "double_comparison.hpp"
#include "my_concepts.hpp"
#include "matrix_iterator.hpp"
#include "exceptions.hpp"

namespace yLAB {

template <my_concepts::numeric_type T>
class LazyMatrix final {
public:
    using size_type              = std::size_t;
    using value_type             = T;
    using pointer                = T*;
    using reference              = T&;
    using const_value_type       = const value_type;
    using const_pointer          = const T*;
    using const_reference        = const T&;
    using iterator               = MatrixIterator<T>;
    using const_iterator         = MatrixIterator<const T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    class ProxyBracket;
    enum class IsZero: bool {Zero, nZero};

    using matrix_size = std::pair<size_type, size_type>;
    using line_info   = std::pair<IsZero, size_type>;
public:
/*------------------------------------------------------------------------------------------------*/
    template <std::forward_iterator ForwIter>
    LazyMatrix(size_type n_line, size_type n_column, ForwIter begin, ForwIter end)
    : LazyMatrix(n_line, n_column) {
        if (static_cast<size_type>(std::distance(begin, end)) != capacity_) {
            throw matrixExcepts::invalidInitMatrixSize();
        }
        std::copy(begin, end, data_.get());
    }

    LazyMatrix(size_type n_line, size_type n_column, const value_type& aggregator = {})
    : n_column_ {n_column},
      n_line_ {n_line},
      capacity_ {n_line * n_column_},
      data_ {new value_type[capacity_]} {
        std::fill(data_.get(), data_.get() + capacity_, aggregator);
    }

    LazyMatrix(size_type n_line, size_type n_column, std::initializer_list<T> ls)
    : LazyMatrix(n_line, n_column, ls.begin(), ls.end()) {}

    LazyMatrix(const LazyMatrix& rhs) noexcept
    : n_column_ {rhs.n_column_},
      n_line_   {rhs.n_line_},
      capacity_ {rhs.capacity_},
      data_     {rhs.data_} {}

    LazyMatrix(LazyMatrix&& rhs) noexcept
    : n_column_ { std::exchange(rhs.n_column_, 0)   },
      n_line_   { std::exchange(rhs.n_line_, 0)     },
      capacity_ { std::exchange(rhs.capacity_, 0)   },
      data_     { std::exchange(rhs.data_, nullptr) } {}


    LazyMatrix& operator=(const LazyMatrix& rhs) noexcept {
        if (this == std::addressof(rhs)) { return *this; };

        LazyMatrix tmp = rhs;
        swap(tmp);

        return *this;
    }

    LazyMatrix& operator=(LazyMatrix&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    LazyMatrix& operator+=(const LazyMatrix& rhs) {
        if (size() != rhs.size()) {
            throw matrixExcepts::invalidMatrixAddition();
        }
        reconstruct();
        std::transform(cbegin(), cend(), rhs.cbegin(), begin(), std::plus<value_type>{});
        return *this;
    }

    LazyMatrix& operator-=(const LazyMatrix& rhs) {
        if (size() != rhs.size()) {
            throw matrixExcepts::invalidMatrixSubstraction();
        }
        reconstruct();
        std::transform(cbegin(), cend(), rhs.cbegin(), begin(), std::minus<value_type>{});
        return *this;
    }

    LazyMatrix& operator*=(value_type coeff) {
        reconstruct();
        std::transform( cbegin(), cend(), begin(), [&coeff](auto&& val) {
                                                     return std::multiplies<value_type>{}(val, coeff);
                                                 } );
        return *this;
    }

    LazyMatrix& operator/=(value_type coeff) {
        if (is_zero(coeff)) {
            throw std::invalid_argument{"trying to divide by 0"};
        }
        reconstruct();
        std::transform( cbegin(), cend(), begin(), [&coeff](auto&& val) {
                                                     return std::divides<value_type>{}(val, coeff);
                                                 } );
        return *this;
    }

    ProxyBracket operator[](size_type index1) {
        reconstruct();
        return ProxyBracket(data_.get() + n_column_ * index1);
    }

    const ProxyBracket operator[](size_type index1) const {
        return ProxyBracket(data_.get() + n_column_ * index1);
    }

    size_type nline() const noexcept { return n_line_; }
    size_type ncolumn() const noexcept { return n_column_; }
    bool is_square() const noexcept { return n_line_ == n_column_; }
    matrix_size size() const noexcept { return {n_line_, n_column_}; }

    void swap_lines(size_type id1, size_type id2) {
        reconstruct();

        size_type offset1 = id1 * n_column_;
        size_type offset2 = id2 * n_column_;
        std::swap_ranges( begin() + offset1, begin() + (offset1 + n_column_),
                          begin() + offset2 );
    }

    void swap(LazyMatrix& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(n_column_, rhs.n_column_);
        std::swap(n_line_, rhs.n_line_);
        std::swap(capacity_, rhs.capacity_);
    }

    LazyMatrix& negate() & {
        reconstruct();
        for (auto& val : *this) {
            val *= -1;
        }
        return *this;
    }

    LazyMatrix& transpose() & {
        reconstruct();
        if (is_square()) {
            return transpose_square();
        }
        auto& m = *this;
        LazyMatrix copy = m;
        std::swap(m.n_line_, m.n_column_);
        for (size_type i = 0; i < n_line_; ++i) {
            for (size_type j = 0; j < n_column_; ++j) {
                m[i][j] = copy[j][i];
            }
        }
        return *this;
    }

    auto determinant() const {
        if (!is_square()) { throw matrixExcepts::invalidDeterminantCall(); }

        if constexpr (std::unsigned_integral<T>) {
            LazyMatrix<double> m {n_line_, n_column_, cbegin(), cend()};
            return m.determinant();
        } else if (std::signed_integral<T>) {
            LazyMatrix m = *this;
            return m.Bareiss();
        } else {
            LazyMatrix m = *this;
            return m.Gauss();
        }
    }

    iterator begin() noexcept {
        reconstruct();
        return construct_iterator(data_.get());
    }

    iterator end() noexcept {
        reconstruct();
        return construct_iterator(data_.get() + capacity_);
    }

    const_iterator cbegin() const noexcept { return construct_iterator(data_.get()); }
    const_iterator cend()   const noexcept { return construct_iterator(data_.get() + capacity_); }
    reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); }
    reverse_iterator rend()   noexcept { return std::make_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(cend()); }
    const_reverse_iterator crend()   const noexcept { return std::make_reverse_iterator(cbegin()); }
private:
    value_type Gauss(); /* Gauss algorithm */
    value_type Bareiss(); /* Bareiss algorithm */

    line_info find_max_column_elem(size_type start_line, size_type column) const {
        auto& matrix = *this;
        std::pair<value_type, size_type> max_pair {value_type {}, 0};
        size_type start_id = start_line;
        for ( ; start_id < n_line_; ++start_id) {
            if (std::abs(matrix[start_id][column]) > std::abs(max_pair.first)) {
                max_pair.first  =  matrix[start_id][column];
                max_pair.second = start_id;
            }
        }
        return !is_zero(max_pair.first) ? line_info{IsZero::nZero, max_pair.second} : line_info{IsZero::Zero, 0};
    }

    line_info find_nzero_column_elem(size_type start_line, size_type column) const {
        auto& matrix = *this;
        for (size_type start_id = start_line; start_id < n_line_; ++start_id) {
            if (!is_zero(matrix[start_id][column])) {
                return {IsZero::nZero, start_id};
            }
        }
        return  {IsZero::Zero, 0};
    }

    bool is_zero(value_type val) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return cmp::is_zero(val);
        } else {
            return val == value_type{0};
        }
    }

    void subtract_lines(size_type line1_id, size_type line2_id, value_type coeff) {
        auto& matrix = *this;
        for (size_type index = line2_id; index < n_column_; ++index) {
           matrix[line1_id][index] -= matrix[line2_id][index] * coeff;
        }
    }

    LazyMatrix& transpose_square() {
        auto& m = *this;
        for (size_type id1 = 0; id1 < n_line_; ++id1) {
            for (size_type id2 = id1; id2 < n_line_; ++id2) {
                std::swap(m[id1][id2], m[id2][id1]);
            }
        }
        return m;
    }

    void reconstruct() {
        if (data_.unique()) { return ; }

        auto save_ptr = data_.get();
        data_.reset(new value_type[capacity_]);
        std::copy(save_ptr, save_ptr + capacity_, data_.get());
    }

    iterator construct_iterator(pointer ptr) noexcept {
        iterator tmp;
        tmp.ptr_ = ptr;
        return tmp;
    }

    const_iterator construct_iterator(const_pointer ptr) const noexcept {
        const_iterator tmp;
        tmp.ptr_ = ptr;
        return tmp;
    }
/*----------------------------------------------------------------------------*/
private:
    size_type n_column_;
    size_type n_line_;
    size_type capacity_;
    std::shared_ptr<value_type[]> data_;

    class ProxyBracket {
    public:
        ProxyBracket(pointer ptr) noexcept
        : line_ptr_ {ptr} {}

        reference operator[](size_type index2) {
            return line_ptr_[index2];
        }

        const_reference operator[](size_type index2) const {
            return line_ptr_[index2];
        }
    private:
        pointer line_ptr_;
    };

}; // <--- class LazyMatrix

template <typename T>
LazyMatrix<T>::value_type LazyMatrix<T>::Gauss() { // Gauss algorithm
    auto& m = *this;
    value_type determ_val {1};
    bool has_sign_changed {false};
    size_type id1 {0};
    for ( ; id1 < (n_line_ - 1); ++id1) {
        auto line_inf = m.find_max_column_elem(id1, id1);
        if (line_inf.first == IsZero::Zero) { return value_type{0}; }
        if (line_inf.second != id1)  {
            m.swap_lines(line_inf.second, id1);
            has_sign_changed = (has_sign_changed + 1) % 2;
        }
        determ_val *= m[id1][id1];
        for (auto substract_id = id1 + 1; substract_id < n_line_; ++substract_id) {
            if (!is_zero(m[substract_id][id1])) {
                auto coeff = m[substract_id][id1] / m[id1][id1];
                m.subtract_lines(substract_id, id1, coeff);
            }
        }
    }
    auto result = determ_val *= m[id1][id1] * (has_sign_changed ? -1 : 1);
    return is_zero(result) ? value_type{0} : result;
}

template <typename T>
LazyMatrix<T>::value_type LazyMatrix<T>::Bareiss() { // Bareiss algorithm
    auto& m = *this;
    bool has_sign_changed {false};
    value_type divider {1};
    size_type k {0};
    for ( ; k < (n_line_ - 1); ++k) {
        for (size_type i = k + 1; i < n_column_; ++i ) {
            for (size_type j = k + 1; j < n_column_; ++j) {
                auto line_inf = m.find_nzero_column_elem(k, k);
                if (line_inf.first == IsZero::Zero) { return value_type{0}; }
                if (line_inf.second != k)  {
                    m.swap_lines(line_inf.second, k);
                    has_sign_changed = (has_sign_changed + 1) % 2;
                }
                m[i][j] = (m[i][j] * m[k][k] - m[i][k] * m[k][j]) / divider;
            }
        }
        divider = m[k][k];
    }
    return m[k][k] * (has_sign_changed ? -1 : 1);
}

template <typename T>
bool operator==(const LazyMatrix<T>& lhs, const LazyMatrix<T>& rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename T>
LazyMatrix<T> operator+(const LazyMatrix<T>& lhs, const LazyMatrix<T>& rhs) {
    auto copy = lhs;
    return copy += rhs;
}

template <typename T>
LazyMatrix<T> operator-(const LazyMatrix<T>& lhs, const LazyMatrix<T>& rhs) {
    auto copy = lhs;
    return copy -= rhs;
}

template <typename T>
LazyMatrix<T> operator*(const LazyMatrix<T>& lhs, typename LazyMatrix<T>::value_type coeff) {
    auto copy = lhs;
    return copy *= coeff;
}

template <typename T>
LazyMatrix<T> operator*(typename LazyMatrix<T>::value_type coeff, const LazyMatrix<T>& rhs) {
    return rhs * coeff;
}

template <typename T>
LazyMatrix<T> operator/(const LazyMatrix<T>& lhs, typename LazyMatrix<T>::value_type coeff) {
    auto copy = lhs;
    return copy /= coeff;
}

template <typename T>
LazyMatrix<T> operator*(const LazyMatrix<T>& lhs, const LazyMatrix<T>& rhs) {
    using size_type = LazyMatrix<T>::size_type;

    if (lhs.ncolumn() != rhs.nline()) { throw matrixExcepts::invalidMatrixMulOperation(); }

    LazyMatrix<T> res(lhs.nline(), rhs.ncolumn());
    for (size_type id1 = 0; id1 < lhs.nline(); ++id1) {
        for (size_type id2 = 0; id2 < rhs.ncolumn(); ++id2) {
            T mul_result {0};
            for (size_type id3 = 0; id3 < rhs.nline(); ++id3) {
                mul_result += lhs[id1][id3] * rhs[id3][id2];
            }
            res[id1][id2] = mul_result;
        }
    }
    return res;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const LazyMatrix<T>& matrix) {
    using size_type = LazyMatrix<T>::size_type;

    auto [n_line, n_column] = matrix.size();
    for (size_type index1 = 0; index1 < n_line; ++index1) {
        for (size_type index2 = 0; index2 < n_column; ++index2) {
            os << matrix[index1][index2] << ' ';
        }
        os << std::endl;
    }
    return os;
}

} // <--- namespace yLAB
