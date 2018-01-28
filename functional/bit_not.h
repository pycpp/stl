//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `bit_not` backport for C++11.
 *
 *  \synopsis
 *      template <typename T = void>
 *      struct bit_not
 *      {
 *          constexpr T operator()(const T& x) const;
 *      };
 *
 *      template <>
 *      struct bit_not<void>
 *      {
 *          using is_transparent = void;
 *
 *          template <typename T>
 *          constexpr implementation-defined operator()(T&& x) const;
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

using std::bit_not;

#else                               // <=CPP11

// OBJECTS
// -------

template <typename T = void>
struct bit_not: std::unary_function<T, T>
{
    PYCPP_CPP14_CONSTEXPR
    T
    operator()(
        const T& x
    )
    const
    {
        return ~x;
    }
};

template <>
struct bit_not<void>
{
    using is_transparent = void;

    template <typename T>
    PYCPP_CPP14_CONSTEXPR
    auto
    operator()(
        T&& x
    )
    const
    noexcept(noexcept(~std::forward<T>(x)))
    -> decltype(~std::forward<T>(x))
    {
        return ~std::forward<T>(x);
    }
};

#endif                              // CPP14

PYCPP_END_NAMESPACE
