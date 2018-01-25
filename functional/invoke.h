//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `invoke*` backports for C++11.
 *
 *  \synopsis
 *      template <typename F, typename ... Ts>
 *      typename std::result_of<F&&(Ts&&...)>::type
 *      invoke(F&& f, Ts&&... ts)
 */

#pragma once

#include <pycpp/stl/type_traits/is_invocable.h>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::invoke;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename F, typename ... Ts>
typename std::result_of<F&&(Ts&&...)>::type
invoke(
    F&& f,
    Ts&&... ts
)
noexcept(noexcept(invoke_impl(std::forward<F>(f), std::forward<Ts>(ts)...)))
{
    return invoke_impl(std::forward<F>(f), std::forward<Ts>(ts)...);
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
