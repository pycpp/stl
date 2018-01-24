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
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_final_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP14)             // CPP14

using std::is_final;

template <typename T>
constexpr bool is_final_v = is_final<T>::value;

#else                               // <=CPP11

// MACROS
// ------

#if defined(HAVE_CLANG)             // HAVE_CLANG
#   if __has_extension(is_final)
#       define PYCPP_IS_FINAL(T) __is_final(T)
#   endif
#elif defined(HAVE_GCC)             // HAVE_GCC
#   if COMPILER_VERSION_CODE >= COMPILER_VERSION(4, 7, 0)
#       define PYCPP_IS_FINAL(T) __is_final(T)
#   endif
#elif defined(HAVE_SUNPRO)          // HAVE_SUNPRO
#   if COMPILER_VERSION_CODE >= COMPILER_VERSION(5, 13, 0)
#       define PYCPP_IS_FINAL(T) __oracle_is_final(T)
#   endif
#elif defined(__is_identifier)      // __is_identifier
#   if __is_identifier(__is_final) == 0
#       define PYCPP_IS_FINAL(T) __is_final(T)
#   endif
#endif                              // HAVE_CLANG

// ALIAS
// -----

#if defined(PYCPP_IS_FINAL)         // PYCPP_IS_FINAL

template <typename T>
struct is_final: std::integral_constant<bool, PYCPP_IS_FINAL(typename std::remove_cv<T>::type)>
{};

#else                               // !PYCPP_IS_FINAL

template <typename T>
struct is_final: std::false_type
{};

#endif                              // PYCPP_IS_FINAL

#endif                              // HAVE_CPP14

PYCPP_END_NAMESPACE
