//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `transform_exclusive_scan` backport for C++11.
 *
 *  \synopsis
 *      template <typename InputIter, typename OutputIter, typename T, typename BinaryOp, typename UnaryOp>
 *      OutputIter transform_exclusive_scan(InputIter first, InputIter last, OutputIter result, T init, BinaryOp b, UnaryOp u);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <numeric>

PYCPP_BEGIN_NAMESPACE

// ALIASES
// -------

// TODO(ahuszagh): Wishlist.
// GCC does not yet have the numeric C++17 implementation yet.
#if defined(PYCPP_CPP17) && !defined(PYCPP_GCC) // CPP17

using std::transform_exclusive_scan;

#else                                           // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename OutputIter, typename T, typename BinaryOp, typename UnaryOp>
inline
OutputIter
transform_exclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    T init,
    BinaryOp b,
    UnaryOp u
)
{
    if (first != last) {
        T saved = init;
        do {
            init = b(init, u(*first));
            *result = saved;
            saved = init;
            ++result;
        } while (++first != last);
    }
    return result;
}

#endif                                          // CP17

PYCPP_END_NAMESPACE
