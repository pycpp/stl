//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `transform_inclusive_scan` backport for C++11.
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

using std::transform_inclusive_scan;

#else                                           // <=CPP14

// FUNCTIONS
// ---------

template <typename InputIter, typename OutputIter, typename T, typename BinaryOp, typename UnaryOp>
OutputIter transform_inclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    BinaryOp b,
    UnaryOp u,
    T init
)
{
    for (; first != last; ++first, (void) ++result) {
        init = b(init, u(*first));
        *result = init;
    }

    return result;
}

template <typename InputIter, typename OutputIter, typename BinaryOp, typename UnaryOp>
OutputIter transform_inclusive_scan(
    InputIter first,
    InputIter last,
    OutputIter result,
    BinaryOp b,
    UnaryOp u
)
{
    if (first != last) {
        typename std::iterator_traits<InputIter>::value_type init = u(*first);
        *result++ = init;
        if (++first != last) {
            return transform_inclusive_scan(first, last, result, b, u, init);
        }
    }

    return result;
}

#endif                                          // CP17

PYCPP_END_NAMESPACE
