//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `rend` backport for C++11.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <initializer_list>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP14)             // CPP14

using std::rend;
using std::crend;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename T, size_t N>
inline CPP17_CONSTEXPR
std::reverse_iterator<T*>
rend(
    T (&arr)[N]
)
{
    return std::reverse_iterator<T*>(arr);
}

template <typename E>
inline CPP17_CONSTEXPR
std::reverse_iterator<const E*>
rend(
    std::initializer_list<E> il
)
{
    return std::reverse_iterator<const E*>(il.begin());
}

template <typename C>
inline CPP17_CONSTEXPR
auto
rend(
    C& c
)
-> decltype(c.rend())
{
    return c.rend();
}

template <typename C>
inline CPP17_CONSTEXPR
auto
rend(
    const C& c
)
-> decltype(c.rend())
{
    return c.rend();
}

template <typename C>
inline CPP17_CONSTEXPR
auto
crend(
    const C& c
)
-> decltype(rend(c))
{
    return rend(c);
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
