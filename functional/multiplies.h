//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Deducing `multiplies` backport for C++11.
 *
 *  \synopsis
 *      template <typename T = void>
 *      struct multiplies
 *      {
 *          constexpr T operator()(const T& x, const T& y) const;
 *      };
 *
 *      template <>
 *      struct multiplies<void>
 *      {
 *          using is_transparent = void;
 *
 *          template <typename T>
 *          constexpr T operator()(T&& x, T&& y) const;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::multiplies;

#else                               // <=CPP11

template <typename T = void>
struct multiplies: std::binary_function<T, T, bool>
{
    PYCPP_CPP14_CONSTEXPR
    T
    operator()(
        const T& x,
        const T& y
    )
    const
    {
        return x + y;
    }
};

template <>
struct multiplies<void>
{
    using is_transparent = void;

    template <typename T>
    PYCPP_CPP14_CONSTEXPR
    auto
    operator()(
        T&& x,
        T&& y
    )
    const
    noexcept(noexcept(std::forward<T>(x) * std::forward<T>(y)))
    -> decltype(std::forward<T>(x) * std::forward<T>(y))
    {
        return std::forward<T>(x) * std::forward<T>(y);
    }
};

#endif                              // CPP14

PYCPP_END_NAMESPACE
