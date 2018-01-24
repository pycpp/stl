//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `void_t` backport for C++11.
 *
 *  \synopsis
 *      template <typename...>
 *      using void_t = void;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)     // CPP17

using std::void_t;

#else                       // <=CPP14

template <typename...>
using void_t = void;

#endif                      // HAVE_CPP17

PYCPP_END_NAMESPACE
