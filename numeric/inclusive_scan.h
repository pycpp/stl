//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `inclusive_scan` backport for C++11.
 *
 *  // TODO: need execution policies....
 *
 *  \synopsis
 *      template <class InputIter, class OutputIter>
 *      OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first);
 *
 *      template <class InputIter, class OutputIter, class BinaryOperation>
 *      OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first, BinaryOperation binary_op);
 *
 *      template <class InputIter, class OutputIter, class BinaryOperation, class T>
 *      OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first, BinaryOperation binary_op, T init);
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

// TODO(ahuszagh): Wishlist.
// GCC does not yet have the numeric C++17 implementation yet.
#if defined(PYCPP_CPP17) && !defined(PYCPP_GCC) // CPP17

using std::inclusive_scan;

#else                                           // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename OutputIter, typename T, typename BinaryOp>
inline
OutputIter
inclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    BinaryOp op,
    T init
)
{
    for (; first != last; ++first, (void) ++result) {
        init = op(init, *first);
        *result = init;
    }
    return result;
}

template <typename InputIter, typename OutputIter, typename BinaryOp>
inline
OutputIter
inclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    BinaryOp op
)
{
    using value_type = typename std::iterator_traits<InputIter>::value_type;
    if (first != last) {
        value_type init = *first;
        *result++ = init;
        if (++first != last)
            return inclusive_scan(first, last, result, op, init);
        }

    return result;
}

template <typename InputIter, typename OutputIter>
inline
OutputIter
inclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result
)
{
    return inclusive_scan(first, last, result, plus<>());
}

#endif                                          // CPP17

PYCPP_END_NAMESPACE
