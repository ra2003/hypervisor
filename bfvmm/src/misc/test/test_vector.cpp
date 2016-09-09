//
// Bareflank Hypervisor
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <test.h>
#include <vector>

void
misc_ut::test_vector_find()
{
    std::vector<int> list = {1, 2, 3};

    this->expect_exception([&] { bfn::find(list, -1); }, ""_ut_ffe);
    this->expect_exception([&] { bfn::find(list, 10); }, ""_ut_ffe);
    this->expect_true(*bfn::find(list, 1) == 2);
}

void
misc_ut::test_vector_cfind()
{
    std::vector<int> list = {1, 2, 3};

    this->expect_exception([&] { bfn::cfind(list, -1); }, ""_ut_ffe);
    this->expect_exception([&] { bfn::cfind(list, 10); }, ""_ut_ffe);
    this->expect_true(*bfn::cfind(list, 1) == 2);
}

void
misc_ut::test_vector_take()
{
    std::vector<int> list = {1, 2, 3};

    this->expect_exception([&] { bfn::take(list, -1); }, ""_ut_ffe);
    this->expect_exception([&] { bfn::take(list, 10); }, ""_ut_ffe);
    this->expect_true(bfn::take(list, 1) == 2);
    this->expect_true(list.size() == 2);
}

void
misc_ut::test_vector_remove()
{
    std::vector<int> list = {1, 2, 3};

    this->expect_exception([&] { bfn::remove(list, -1); }, ""_ut_ffe);
    this->expect_exception([&] { bfn::remove(list, 10); }, ""_ut_ffe);
    this->expect_no_exception([&] { bfn::remove(list, 1); });
    this->expect_true(list.size() == 2);
}

// /// Find
// ///
// /// Get the iterator from the provided vector, given an index.
// ///
// /// expects: index >= 0
// /// expects: index < v.size()
// /// ensures: ret != v.end()
// ///
// /// @param v std::vector to get iterator from
// /// @param index the iterator to locate
// /// @return returns the iterator at pos == index, or throws gsl::fail_fast
// ///
// template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
// auto find(std::vector<T> &v, const I index)
// {
//     // [[ensures ret: ret != v.end()]]
//     expects(index >= 0 && index < v.size())

//     return v.begin() + gsl::narrow_cast<typename std::vector<T>::difference_type>(index);
// }

// /// Find (const)
// ///
// /// Get the iterator from the provided vector, given an index.
// ///
// /// expects: index >= 0
// /// expects: index < v.size()
// /// ensures: ret != v.end()
// ///
// /// @param v std::vector to get iterator from
// /// @param index the iterator to locate
// /// @return returns the iterator at pos == index, or throws gsl::fail_fast
// ///
// template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
// const auto cfind(std::vector<T> &v, const I index)
// {
//     // [[ensures ret: ret != v.end()]]
//     expects(index >= 0 && index < v.size())

//     return v.cbegin() + gsl::narrow_cast<typename std::vector<T>::difference_type>(index);
// }

// /// Remove
// ///
// /// Removes an element from the provided vector. This function uses
// /// std::vector::erase, and thus, all iterators are invalidated after this
// /// function call is made.
// ///
// /// expects: index >= 0
// /// expects: index < v.size()
// /// ensures:
// ///
// /// @param v std::vector to get iterator from
// /// @param index the iterator to locate
// ///
// template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
// void remove(std::vector<T> &v, const I index)
// {
//     v.erase(cfind(v, index));
// }

// /// Take
// ///
// /// Takes an element from the provided vector. This function uses
// /// std::vector::erase, and thus, all iterators are invalidated after this
// /// function call is made.
// ///
// /// expects: index >= 0
// /// expects: index < v.size()
// /// ensures:
// ///
// /// @param v std::vector to get iterator from
// /// @param index the iterator to locate
// /// @return returns the element that was removed
// ///
// template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
// auto take(std::vector<T> &v, const I index)
// {
//     const auto iter = cfind(v, index);

//     auto val = *iter;
//     v.erase(iter);

//     return val;
// }
