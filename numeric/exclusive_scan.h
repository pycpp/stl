//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `exclusive_scan` backport for C++11.
 *
 *  // TODO: need execution policies....
 *
 *  \synopsis
 *      template <typename InputIter, typename OutputIter, typename T>
 *      OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init);
 *
 *      template <typename InputIter, typename OutputIter, typename T, typename BinaryOperation>
 *      OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init, BinaryOperation binary_op);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional/plus.h>
#include <numeric>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::exclusive_scan;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename OutputIter, typename T, typename BinaryOp>
inline
OutputIter
exclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    T init,
    BinaryOp op
)
{
    if (first != last) {
        T saved = init;
        do {
            init = op(init, *first);
            *result = saved;
            saved = init;
            ++result;
        } while (++first != last);
    }
    return result;
}

template <typename InputIter, typename OutputIter, typename T>
inline
OutputIter
exclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    T init
)
{
    return exclusive_scan(first, last, result, init, plus<>());
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
