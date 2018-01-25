//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Deducing `logical_not` backport for C++11.
 *
 *  \synopsis
 *      template <typename T = void>
 *      struct logical_not
 *      {
 *          constexpr bool operator()(const T& x) const;
 *      };
 *
 *      template <>
 *      struct logical_not<void>
 *      {
 *          using is_transparent = void;
 *
 *          template <typename T>
 *          constexpr bool operator()(T&& x) const;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP14)             // CPP14

using std::logical_not;

#else                               // <=CPP11

template <typename T = void>
struct logical_not: std::binary_function<T, T, bool>
{
    CPP14_CONSTEXPR
    bool
    operator()(
        const T& x
    )
    const
    {
        return -x;
    }
};

template <>
struct logical_not<void>
{
    using is_transparent = void;

    template <typename T>
    CPP14_CONSTEXPR
    auto
    operator()(
        T&& x
    )
    const
    noexcept(noexcept(!std::forward<T>(x)))
    -> decltype(!std::forward<T>(x))
    {
        return !std::forward<T>(x);
    }
};

#endif                              // CPP14

PYCPP_END_NAMESPACE
