//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `exchange` backport for C++11.
 *
 *  \synopsis
 *      template <class T>
 *      constexpr std::add_const_t<T>& as_const(T& t) noexcept;
 *
 *      template <class T>
 *      void as_const(const T&&) = delete;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::as_const;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename T>
constexpr
typename std::add_const<T>::type&
as_const(T& t)
noexcept
{
    return t;
}

template <typename T>
void
as_const(const T&&) = delete;

#endif                              // CPP17

PYCPP_END_NAMESPACE
