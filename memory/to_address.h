//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `to_address` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      constexpr T* to_address(T* p) noexcept;
 *
 *      template <typename Pointer>
 *      decltype(auto) to_address(const Pointer& p) noexcept;
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/memory/to_raw_pointer.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP20)     // CPP20

using std::to_address;

#else                       // <=CPP17

template <typename T>
inline constexpr
T*
to_address(T* p)
noexcept
{
    static_assert(!std::is_function<T>::value, "T is a function type");
    return p;
}

template <typename Pointer>
inline
auto
to_address(const Pointer& p)
noexcept
-> decltype(to_raw_pointer(p))
{
    return to_raw_pointer(p);
}

#endif                      // HAVE_CPP20

PYCPP_END_NAMESPACE
