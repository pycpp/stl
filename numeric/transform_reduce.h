//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `transform_reduce` backport for C++11.
 *
 *  \synopsis
 *      template <typename InputIter, typename T, typename BinaryOp, typename UnaryOp>
 *      T transform_reduce(InputIter first, InputIter last, T init, BinaryOp op, UnaryOp u);
 *
 *      template <typename InputIter1, typename InputIter2, typename T, typename BinaryOp1, typename BinaryOp2>
 *      T transform_reduce(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init, BinaryOp1 op1, BinaryOp2 op2);
 *
 *      template <typename InputIter1, typename InputIter2, typename T>
 *      T transform_reduce(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional/multiplies.h>
#include <pycpp/stl/functional/plus.h>
#include <iterator>
#include <numeric>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// TODO(ahuszagh): Wishlist.
// GCC does not yet have the numeric C++17 implementation yet.
#if defined(PYCPP_CPP17) && !defined(PYCPP_GCC) // CPP17

using std::transform_reduce;

#else                                           // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename T, typename BinaryOp, typename UnaryOp>
inline
T
transform_reduce(
    InputIter first,
    InputIter last,
    T init,
    BinaryOp op,
    UnaryOp u
)
{
    for (; first != last; ++first) {
        init = op(init, u(*first));
    }
    return init;
}

template <
    typename InputIter1,
    typename InputIter2,
    typename T,
    typename BinaryOp1,
    typename BinaryOp2
>
inline
T
transform_reduce(
    InputIter1 first1,
    InputIter1 last1,
    InputIter2 first2,
    T init,
    BinaryOp1 op1,
    BinaryOp2 op2
)
{
    for (; first1 != last1; ++first1, (void) ++first2) {
        init = op1(init, op2(*first1, *first2));
    }
    return init;
}

template <typename InputIter1, typename InputIter2, typename T>
inline
T
transform_reduce(
    InputIter1 first1,
    InputIter1 last1,
    InputIter2 first2,
    T init
)
{
    return transform_reduce(first1, last1, first2, std::move(init), plus<>(), multiplies<>());
}

#endif                                          // CPP17

PYCPP_END_NAMESPACE
