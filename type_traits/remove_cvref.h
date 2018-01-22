//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `remove_cvref` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      using remove_cvref = implementation-defined;
 *
 *      template <typename T>
 *      using remove_cvref_t = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP20)             // CPP20

using std::remove_cvref;

#else                               // <=CPP17

template <typename T>
struct remove_cvref
{
    using type = typename std::remove_cv<
        typename std::remove_reference<T>::type
    >::type;
};

#endif                              // HAVE_CPP20

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

PYCPP_END_NAMESPACE
