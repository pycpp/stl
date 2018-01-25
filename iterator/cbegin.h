//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `cbegin` backport for C++11.
 *
 *  \synopsis
 *      template <typename C>
 *      constexpr decltype(auto) cbegin(const C& c);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP14)             // CPP14

using std::cbegin;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename C>
inline CPP14_CONSTEXPR
auto
cbegin(
    const C& c
)
-> decltype(std::begin(c))
{
    return std::begin(c);
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
