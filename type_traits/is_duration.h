//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `chrono::duration` type detection.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_duration = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_duration_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/type_traits/remove_cvref.h>
#include <chrono>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
struct is_duration_impl: std::false_type
{};

template <typename Rep, typename Period>
struct is_duration_impl<std::chrono::duration<Rep, Period>>: std::true_type
{};

template <typename T>
struct is_duration: is_duration_impl<remove_cvref_t<T>>
{};

#if defined(PYCPP_CPP14)            // CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_duration_v = is_duration<T>::value;

#endif

PYCPP_END_NAMESPACE
