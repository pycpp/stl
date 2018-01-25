//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `unitinialized_*` backports for C++11 utilities.
 *
 *  \synopsis
 *      template <typename InputIter, typename ForwardIter>
 *      ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter first_res);
 *
 *      template <typename InputIter, typename Size, typename ForwardIter>
 *      std::pair<InputIter, ForwardIter> uninitialized_move_n(InputIter first, Size n, ForwardIter first_res);
 *
 *      template <typename ForwardIter>
 *      void uninitialized_default_construct(ForwardIter first, ForwardIter last);
 *
 *      template <typename ForwardIter, typename Size>
 *      ForwardIter uninitialized_default_construct_n(ForwardIter first, Size n);
 *
 *      template <typename ForwardIter>
 *      void uninitialized_value_construct(ForwardIter first, ForwardIter last);
 *
 *      template <typename ForwardIter, typename Size>
 *      ForwardIter uninitialized_value_construct_n(ForwardIter first, Size n);
 */

#pragma once

#include <pycpp/stl/memory/destroy.h>
#include <iterator>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)     // HAVE_CPP17

using std::uninitialized_move;
using std::uninitialized_move_n;
using std::uninitialized_default_construct;
using std::uninitialized_default_construct_n;
using std::uninitialized_value_construct;
using std::uninitialized_value_construct_n;

#else                       // !HAVE_CPP17

// FUNCTIONS
// ---------

template <typename InputIter, typename ForwardIter>
inline
ForwardIter
uninitialized_move(
    InputIter first,
    InputIter last,
    ForwardIter first_res
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first_res;
    try {
        for (; first != last; (void)++idx, ++first) {
            ::new((void*)std::addressof(*idx)) value_type(std::move(*first));
        }
        return idx;
    } catch (...) {
        destroy(first_res, idx);
        throw;
    }
}

template <typename InputIter, typename Size, typename ForwardIter>
inline
std::pair<InputIter, ForwardIter>
uninitialized_move_n(
    InputIter first,
    Size n,
    ForwardIter first_res
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first_res;
    try {
        for (; n > 0; ++idx, (void)++first, --n) {
            ::new((void*)std::addressof(*idx)) value_type(std::move(*first));
        }
        return {first, idx};
    } catch (...) {
        destroy(first_res, idx);
        throw;
    }
}

template <typename ForwardIter>
inline
void
uninitialized_default_construct(
    ForwardIter first,
    ForwardIter last
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first;
    try {
        for (; idx != last; ++idx) {
            ::new((void*)std::addressof(*idx)) value_type;
        }
    } catch (...) {
        destroy(first, idx);
        throw;
    }
}

template <typename ForwardIter, typename Size>
inline
ForwardIter
uninitialized_default_construct_n(
    ForwardIter first,
    Size n
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first;
    try {
        for (; n > 0; (void)++idx, --n) {
            ::new((void*)std::addressof(*idx)) value_type;
        }
        return idx;
    } catch (...) {
        destroy(first, idx);
        throw;
    }
}


template <typename ForwardIter>
inline
void
uninitialized_value_construct(
    ForwardIter first,
    ForwardIter last
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first;
    try {
        for (; idx != last; ++idx) {
            ::new((void*)std::addressof(*idx)) value_type();
        }
    } catch (...) {
        destroy(first, idx);
        throw;
    }
}

template <typename ForwardIter, typename Size>
inline
ForwardIter
uninitialized_value_construct_n(
    ForwardIter first,
    Size n
)
{
    using value_type = typename std::iterator_traits<ForwardIter>::value_type;
    auto idx = first;
    try {
        for (; n > 0; (void)++idx, --n) {
            ::new((void*)std::addressof(*idx)) value_type();
        }
        return idx;
    } catch (...) {
        destroy(first, idx);
        throw;
    }
}

#endif                      // HAVE_CPP17

PYCPP_END_NAMESPACE
