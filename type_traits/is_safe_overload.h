//  :copyright: (c) 2016 Facebook, Inc. All rights reserved.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: BSD 3-Clause, see licenses/bsd-3.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief C++17 backport of swappable detection.
 *
 *  Check if the variadic template is not the same as the initial
 *  class, to allow variadic template overloads for everything but
 *  the same class.
 *
 *  A simple example is using `is_safe_overload` with a variadic
 *  template constructor while still allowing the copy constructor.
 *
 *  \code
 *      class my_struct
 *      {
 *          my_struct() = default;
 *          my_struct(const my_struct&) = default;
 *
 *          template <typename ... Ts, typename = is_safe_overload<true, my_struct, Ts...>>
 *          my_struct(Ts&&... ts);
 *      };
 *
 *  \synopsis
 *      template <bool RemoveReference, typename T, typename ... Ts>
 *      struct is_safe_overload;
 *
 *      template <bool RemoveReference, typename T, typename ... Ts>
 *      using enable_safe_overload = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <bool RemoveReference, typename T, typename ... Ts>
 *      constexpr bool is_safe_overload_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// TYPE

// By default, the overload is safe.
template <bool, typename, typename ...>
struct is_safe_overload_impl
{
    using type = std::true_type;
};


// Disable the overload if the first argument is the same or a derived
// type of the current class, after removing references and CV
// qualifers.
//
// This differs from Facebook's fatal implementation, in that
// if the first argument in the variadic pack is the class, we
// disable the safe overload, as opposed to fatal, which disables
// the overload if the only argument in the pack is the class.
//
// This is useful for C++11 semantics, such as copy-constructors and
// move constructors that take custom allocators.
// For example, fatal would not work with the following two overloads,
// while this implementation works:
//
//      class my_struct
//      {
//          using allocator_type = std::allocator<char>;
//          my_struct() = default;
//          my_struct(const my_struct&, const allocator_type& alloc = allocator_type());
//
//          template <typename ... Ts, typename = is_safe_overload<true, my_struct, Ts...>>
//          my_struct(Ts&&... ts);
//      };
//
template <bool RemoveReference, typename Class, typename T, typename ... Ts>
struct is_safe_overload_impl<RemoveReference, Class, T, Ts...>
{
    using class_type = Class;
    using overload_type = typename std::remove_cv<
        typename std::conditional<
            RemoveReference,
            typename std::remove_reference<T>::type,
            T
        >::type
    >::type;

    // use std::is_same and std::is_base_of, since std::is_base_of
    // fails for primitive types.
    using type = std::integral_constant<
        bool,
        !(
            std::is_base_of<class_type, overload_type>::value ||
            std::is_same<class_type, overload_type>::value
        )
    >;
};


template <bool RemoveReference, typename T, typename ... Ts>
struct is_safe_overload: is_safe_overload_impl<RemoveReference, T, Ts...>::type
{};

// ENABLE IF

template <bool RemoveReference, typename T, typename ... Ts>
using enable_safe_overload = std::enable_if<
    is_safe_overload<RemoveReference, T, Ts...>::value
>;

template <bool RemoveReference, typename T, typename ... Ts>
using enable_safe_overload_t = typename enable_safe_overload<RemoveReference, T, Ts...>::type;

#ifdef PYCPP_CPP14

// SFINAE
// ------

template <bool RemoveReference, typename T, typename ... Ts>
constexpr bool is_safe_overload_v = is_safe_overload<RemoveReference, T, Ts...>::value;

#endif

PYCPP_END_NAMESPACE
