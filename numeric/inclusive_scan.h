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

#if defined(PYCPP_CPP17)            // CPP17

using std::inclusive_scan;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIterator, typename OutputIterator, typename T, typename BinaryOp>
inline
OutputIterator
inclusive_scan(
    InputIterator first,
    InputIterator last,
    OutputIterator result,
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

template <typename InputIterator, typename OutputIterator, typename BinaryOp>
inline
OutputIterator
inclusive_scan(
    InputIterator first,
    InputIterator last,
    OutputIterator result,
    BinaryOp op
)
{
    using value_type = typename std::iterator_traits<InputIterator>::value_type;
    if (first != last) {
        value_type init = *first;
        *result++ = init;
        if (++first != last)
            return inclusive_scan(first, last, result, op, init);
        }

    return result;
}

template <typename InputIterator, typename OutputIterator>
inline
OutputIterator
inclusive_scan(
    InputIterator first,
    InputIterator last,
    OutputIterator result
)
{
    return inclusive_scan(first, last, result, plus<>());
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
