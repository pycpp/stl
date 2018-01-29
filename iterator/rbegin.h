//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `rbegin` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, size_t N>
 *      constexpr
 *      reverse_iterator<T*>
 *      rbegin(T (&arr)[N]);
 *
 *      template <typename E>
 *      constexpr reverse_iterator<const E*>
 *      rbegin(initializer_list<E> il);
 *
 *      template <typename C>
 *      constexpr decltype(auto) rbegin(C& c);
 *
 *      template <typename C>
 *      constexpr decltype(auto) rbegin(const C& c);
 *
 *      template <typename C>
 *      constexpr decltype(auto) crbegin(const C& c);
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

using std::rbegin;
using std::crbegin;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename T, size_t N>
inline PYCPP_CPP17_CONSTEXPR
std::reverse_iterator<T*>
rbegin(
    T (&arr)[N]
)
{
    return std::reverse_iterator<T*>(arr + N);
}

template <typename E>
inline PYCPP_CPP17_CONSTEXPR
std::reverse_iterator<const E*>
rbegin(
    std::initializer_list<E> il
)
{
    return std::reverse_iterator<const E*>(il.end());
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
auto
rbegin(
    C& c
)
-> decltype(c.rbegin())
{
    return c.rbegin();
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
auto
rbegin(
    const C& c
)
-> decltype(c.rbegin())
{
    return c.rbegin();
}

template <typename C>
inline PYCPP_CPP17_CONSTEXPR
auto
crbegin(
    const C& c
)
-> decltype(rbegin(c))
{
    return rbegin(c);
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
