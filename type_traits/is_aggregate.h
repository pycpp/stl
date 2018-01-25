//  :copyright: (c) 2014 Agustin Berge.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `is_aggregate` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      struct is_aggregate: implementation-defined
 *      {};
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_aggregate_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/abi.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::is_aggregate;

#else                               // <=CPP14

// MACROS
// ------

#if defined(HAVE_CLANG)             // HAVE_CLANG
#   if __has_extension(is_aggregate)
#       define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#   endif
#elif defined(HAVE_GNUC)            // HAVE_GNUC
#   if GNUC_MAJOR_VERSION >= 7
#       define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#   endif
#elif defined(__is_identifier)      // __is_identifier
#   if __is_identifier(__is_aggregate) == 0
#       define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#   endif
#endif                              // HAVE_CLANG

// ALIAS
// -----

#if defined(PYCPP_IS_AGGREGATE)      // PYCPP_IS_AGGREGATE

template <typename T>
struct is_aggregate: std::integral_constant<bool, PYCPP_IS_AGGREGATE(typename std::remove_cv<T>::type)>
{};

#else                               // !PYCPP_IS_AGGREGATE

template <typename T>
struct is_aggregate: std::false_type
{};

#endif                              // PYCPP_IS_AGGREGATE

#endif                              // HAVE_CPP17

#if defined(HAVE_CPP14)

template <typename T>
constexpr bool is_aggregate_v = is_aggregate<T>::value;

#endif                              // HAVE_CPP14

PYCPP_END_NAMESPACE
