//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `cend` backport for C++11.
 *
 *  \synopsis
 *      template <typename C>
 *      constexpr decltype(auto) cend(const C& c);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::cend;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename C>
inline PYCPP_CPP14_CONSTEXPR
auto
cend(
    const C& c
)
-> decltype(std::end(c))
{
    return std::end(c);
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
