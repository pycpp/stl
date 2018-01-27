//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for `reference_wrapper`.
 *
 *  Detect if a type is a `std::reference_wrapper`.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_reference_wrapper = implementation-defined;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_reference_wrapper_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <functional>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
struct is_reference_wrapper_impl: std::false_type
{};

template <typename T>
struct is_reference_wrapper_impl<std::reference_wrapper<T>>: std::true_type
{};

template <typename T>
struct is_reference_wrapper:
    is_reference_wrapper_impl<typename std::remove_cv<typename std::remove_reference<T>::type>::type>
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

#endif

PYCPP_END_NAMESPACE
