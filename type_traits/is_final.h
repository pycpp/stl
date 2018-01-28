//  :copyright: (c) 2014 Agustin Berge.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `is_final` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_final = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_final_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::is_final;

template <typename T>
constexpr bool is_final_v = is_final<T>::value;

#else                               // <=CPP11

// MACROS
// ------

#if defined(PYCPP_CLANG)                        // CLANG
#   if PYCPP_HAS_EXTENSION(is_final)
#       define PYCPP_IS_FINAL(T) __is_final(T)
#   endif
#elif defined(PYCPP_GCC)                        // GCC
#   if PYCPP_COMPILER_VERSION_CODE >= PYCPP_COMPILER_VERSION(4, 7, 0)
#       define PYCPP_IS_FINAL(T) __is_final(T)
#   endif
#elif defined(PYCPP_SUNPRO)                     // SUNPRO
#   if PYCPP_COMPILER_VERSION_CODE >= PYCPP_COMPILER_VERSION(5, 13, 0)
#       define PYCPP_IS_FINAL(T) __oracle_is_final(T)
#   endif
#elif PYCPP_IS_IDENTIFIER(__is_final) == 0      // __is_identifier(__is_final)
#   define PYCPP_IS_FINAL(T) __is_final(T)
#endif                                          // CLANG

// ALIAS
// -----

#if defined(PYCPP_IS_FINAL)         // IS_FINAL

template <typename T>
struct is_final: std::integral_constant<bool, PYCPP_IS_FINAL(typename std::remove_cv<T>::type)>
{};

#else                               // !IS_FINAL

template <typename T>
struct is_final: std::false_type
{};

#endif                              // IS_FINAL

#endif                              // CPP14

PYCPP_END_NAMESPACE
