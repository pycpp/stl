//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Deducing `logical_or` backport for C++11.
 *
 *  \synopsis
 *      template <typename T = void>
 *      struct logical_or
 *      {
 *          constexpr bool operator()(const T& x, const T& y) const;
 *      };
 *
 *      template <>
 *      struct logical_or<void>
 *      {
 *          using is_transparent = void;
 *
 *          template <typename T1, typename T2>
 *          constexpr bool operator()(T1&& x, T2&& y) const;
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

using std::logical_or;

#else                               // <=CPP11

template <typename T = void>
struct logical_or: std::binary_function<T, T, bool>
{
    CPP14_CONSTEXPR
    bool
    operator()(
        const T& x,
        const T& y
    )
    const
    {
        return x == y;
    }
};

template <>
struct logical_or<void>
{
    using is_transparent = void;

    template <typename T1, typename T2>
    CPP14_CONSTEXPR
    auto
    operator()(
        T1&& x,
        T2&& y
    )
    const
    noexcept(noexcept(std::forward<T1>(x) || std::forward<T2>(y)))
    -> decltype(std::forward<T1>(x) || std::forward<T2>(y))
    {
        return std::forward<T1>(x) || std::forward<T2>(y);
    }
};

#endif                              // CPP14

PYCPP_END_NAMESPACE
