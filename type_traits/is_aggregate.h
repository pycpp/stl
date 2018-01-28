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
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_aggregate_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/abi.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::is_aggregate;

#else                               // <=CPP14

// MACROS
// ------

#if defined(PYCPP_CLANG)                        // CLANG
#   if PYCPP_HAS_EXTENSION(is_aggregate)
#       define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#   endif
#elif defined(PYCPP_GNUC)                       // GNUC
#   if PYCPP_GNUC_MAJOR_VERSION >= 7
#       define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#   endif
#elif PYCPP_IS_IDENTIFIER(__is_aggregate) == 0  // __is_identifier
#   define PYCPP_IS_AGGREGATE(T) __is_aggregate(T)
#endif                                          // CLANG

// ALIAS
// -----

#if defined(PYCPP_IS_AGGREGATE)      // IS_AGGREGATE

template <typename T>
struct is_aggregate: std::integral_constant<bool, PYCPP_IS_AGGREGATE(typename std::remove_cv<T>::type)>
{};

#else                               // !IS_AGGREGATE

template <typename T>
struct is_aggregate: std::false_type
{};

#endif                              // PYCPP_IS_AGGREGATE

#endif                              // CPP17

#if defined(PYCPP_CPP14)

template <typename T>
constexpr bool is_aggregate_v = is_aggregate<T>::value;

#endif                              // CPP14

PYCPP_END_NAMESPACE
