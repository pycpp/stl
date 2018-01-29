//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `rend` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, size_t N>
 *      constexpr
 *      reverse_iterator<T*>
 *      rend(T (&arr)[N]);
 *
 *      template <typename E>
 *      constexpr reverse_iterator<const E*>
 *      rend(initializer_list<E> il);
 *
 *      template <typename C>
 *      constexpr decltype(auto) rend(C& c);
 *
 *      template <typename C>
 *      constexpr decltype(auto) rend(const C& c);
 *
 *      template <typename C>
 *      constexpr decltype(auto) crend(const C& c);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <initializer_list>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::rend;
using std::crend;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename T, size_t N>
inline PYCPP_CPP17_CONSTEXPR
std::reverse_iterator<T*>
rend(
    T (&arr)[N]
)
{
    return std::reverse_iterator<T*>(arr);
}

template <typename E>
inline PYCPP_CPP17_CONSTEXPR
std::reverse_iterator<const E*>
rend(
    std::initializer_list<E> il
)
{
    return std::reverse_iterator<const E*>(il.begin());
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
auto
rend(
    C& c
)
-> decltype(c.rend())
{
    return c.rend();
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
auto
rend(
    const C& c
)
-> decltype(c.rend())
{
    return c.rend();
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
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
