//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect if allocator has `is_always_equal` member type.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      struct has_is_always_equal;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool has_is_always_equal_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/stl/type_traits/has_member_type.h>
#include <pycpp/preprocessor/compiler.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_TYPE(is_always_equal, has_is_always_equal);

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_is_always_equal_v = has_is_always_equal<T>::value;

#endif              // HAVE_CPP14

PYCPP_END_NAMESPACE
