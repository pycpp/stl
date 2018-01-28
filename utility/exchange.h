//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `exchange` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, typename U = T>
 *      constexpr T exchange(T& obj, U&& new_value);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::exchange;

#else                               // <=CPP11

// FUNCTIONS
// ---------

template <typename T, typename U = T>
inline PYCPP_CPP14_CONSTEXPR
T
exchange(
    T& obj,
    U&& new_value
)
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

#endif                              // CPP14

PYCPP_END_NAMESPACE
