//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `reduce` backport for C++11.
 *
 *  \synopsis
 *      template <typename InputIter, typename T, typename BinaryOp>
 *      T reduce(InputIter first, InputIter last, T init, BinaryOp op);
 *
 *      template <typename InputIter, typename T>
 *      T reduce(InputIter first, InputIter last, T init);
 *
 *      template <typename InputIter>
 *      typename std::iterator_traits<InputIter>::value_type
 *      reduce(InputIter first, InputIter last);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional/plus.h>
#include <iterator>
#include <numeric>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::reduce;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename T, typename BinaryOp>
inline
T
reduce(
    InputIter first,
    InputIter last,
    T init,
    BinaryOp op
)
{
    for (; first != last; ++first)
        init = op(init, *first);
    return init;
}

template <typename InputIter, typename T>
inline
T
reduce(
    InputIter first,
    InputIter last,
    T init
)
{
    return reduce(first, last, init, plus<>());
}

template <typename InputIter>
inline
typename std::iterator_traits<InputIter>::value_type
reduce(
    InputIter first,
    InputIter last
)
{
    using value_type = typename std::iterator_traits<InputIter>::value_type;
    return reduce(first, last, value_type());
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
