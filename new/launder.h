//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `launder` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      [[ nodiscard ]] constexpr T* launder(T* p) noexcept;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <new>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::launder;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename T>
PYCPP_CPP17_NODISCARD
inline constexpr T*
launder(T* p)
noexcept
{
    static_assert(!std::is_function<T>::value, "can't launder functions");
    static_assert(!std::is_same<void, typename std::remove_cv<T>::type>::value, "can't launder cv-void");

#if PYCPP_HAS_BUILTIN(__builtin_launder)
    return __builtin_launder(p);
#elif defined(PYCPP_GNUC) && (PYCPP_COMPILER_MAJOR_VERSION >= 7)
    return __builtin_launder(p);
#else
    return p;
#endif
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
