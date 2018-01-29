//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `make_reverse_iterator` backport for C++11.
 *
 *  \synopsis
 *      template <typename Iter>
 *      reverse_iterator<Iter> make_reverse_iterator(Iter i);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::make_reverse_iterator;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename Iter>
inline PYCPP_CPP17_CONSTEXPR
std::reverse_iterator<Iter>
make_reverse_iterator(
    Iter i
)
{
    return std::reverse_iterator<Iter>(i);
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
