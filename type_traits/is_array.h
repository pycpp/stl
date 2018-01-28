//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for C-style arrays.
 *
 *  Detect if a type is a C-style array. Mimics `std::is_array`'s
 *  specialization for `const` and `volatile`, but does not remove
 *  references. `is_array` detects both bounded and unbounded arrays,
 *  while `is_bounded_array` only detects bounded (`T[N]`) arrays
 *  and `is_unbounded_array` only detects unbounded (`T[]`) arrays.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_array = implementation-defined;
 *
 *      template <typename T>
 *      using is_bounded_array = implementation-defined;
 *
 *      template <typename T>
 *      using is_unbounded_array = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_array_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_bounded_array_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_unbounded_array_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIASES
// -------

using std::is_array;

// SFINAE
// ------

// IS BOUNDED ARRAY

template <typename T>
struct is_bounded_array_impl: std::false_type
{};

template <typename T, std::size_t N>
struct is_bounded_array_impl<T[N]>: std::true_type
{};

template <typename T>
struct is_bounded_array: is_bounded_array_impl<typename std::remove_cv<T>::type>
{};

// IS UNBOUNDED ARRAY

template <typename T>
struct is_unbounded_array_impl: std::false_type
{};

template <typename T>
struct is_unbounded_array_impl<T[]>: std::true_type
{};

template <typename T>
struct is_unbounded_array: is_unbounded_array_impl<typename std::remove_cv<T>::type>
{};

#if defined(PYCPP_CPP14)

// SFINAE
// ------

template <typename T>
constexpr bool is_array_v = is_array<T>::value;

template <typename T>
constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <typename T>
constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

#endif

PYCPP_END_NAMESPACE
