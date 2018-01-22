//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Logical type trait backports for C++11.
 *
 *  \synopsis
 *      template <template <typename> class T, typename ... Ts>
 *      struct type_map_and: implementation-defined
 *      {};
 *
 *      template <template <typename> class T, typename ... Ts>
 *      struct type_map_or: implementation-defined
 *      {};
 *
 *      template <template <typename> class T, typename U>
 *      struct type_not: implementation-defined
 *      {};
 *
 *      template <typename ... Ts>
 *      struct map_and: implementation-defined
 *      {};
 *
 *      template <typename ... Ts>
 *      struct map_or: implementation-defined
 *      {};
 *
 *      template <typename T>
 *      using not_ = implementation-defined;
 *
 *      template <typename ... Ts>
 *      using conjunction = implementation-defined;
 *
 *      template <typename ... Ts>
 *      using disjunction = implementation-defined;
 *
 *      template <typename T>
 *      using negation = implementation-defined;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <template <typename> class T, typename ... Ts>
 *      constexpr bool type_map_and_v = implementation-defined;
 *
 *      template <template <typename> class T, typename ... Ts>
 *      constexpr bool type_map_or_v = implementation-defined;
 *
 *      template <template <typename> class T, typename U>
 *      constexpr bool type_not_v = implementation-defined;
 *
 *      template <typename ... Ts>
 *      constexpr bool map_and_v = implementation-defined;
 *
 *      template <typename ... Ts>
 *      constexpr bool map_or_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool not_v = implementation-defined;
 *
 *      template <typename ... Ts>
 *      constexpr bool conjunction_v = implementation-defined;
 *
 *      template <typename ... Ts>
 *      constexpr bool disjunction_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool negation_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/type_traits/identity.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

// TYPE MAP AND IMPL

template <template <typename> class T, typename ... Ts>
struct type_map_and_impl;

template <template <typename> class T>
struct type_map_and_impl<T>: std::true_type
{};

template <template <typename> class T, typename U, typename ... Ts>
struct type_map_and_impl<T, U, Ts...>:
    std::integral_constant<
        bool,
        T<U>::value && type_map_and_impl<T, Ts...>::value
    >::type
{};

// MAP AND IMPL

template <typename ... Ts>
struct map_and_impl: type_map_and_impl<identity_t, Ts...>
{};

// TYPE MAP OR IMPL

template <template <typename> class T, typename ... Ts>
struct type_map_or_impl;

template <template <typename> class T>
struct type_map_or_impl<T>: std::false_type
{};

template <template <typename> class T, typename U, typename ... Ts>
struct type_map_or_impl<T, U, Ts...>:
    std::integral_constant<
        bool,
        T<U>::value || type_map_or_impl<T, Ts...>::value
    >::type
{};

// MAP OR IMPL

template <typename ... Ts>
struct map_or_impl: type_map_or_impl<identity_t, Ts...>
{};

// TYPE NOT IMPL

template <template <typename> class T, typename U>
struct type_not_impl:
    std::integral_constant<
        bool,
        !T<U>::value
    >::type
{};

// NOT IMPL

template <typename T>
struct not_impl: type_not_impl<identity_t, T>
{};

// API
// ---

template <template <typename> class T, typename ... Ts>
struct type_map_and: type_map_and_impl<T, Ts...>
{};

template <template <typename> class T, typename ... Ts>
struct type_map_or: type_map_or_impl<T, Ts...>
{};

template <template <typename> class T, typename U>
struct type_not: type_not_impl<T, U>
{};

template <typename ... Ts>
struct map_and: map_and_impl<Ts...>
{};

template <typename ... Ts>
struct map_or: map_or_impl<Ts...>
{};

template <typename T>
struct not_: not_impl<T>
{};

template <typename ... Ts>
struct conjucation: map_and<Ts...>
{};

template <typename ... Ts>
struct disjunction: map_or<Ts...>
{};

template <typename T>
struct negation: not_impl<T>
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <template <typename> class T, typename ... Ts>
constexpr bool type_map_and_v = type_map_and<T, Ts...>::value;

template <template <typename> class T, typename ... Ts>
constexpr bool type_map_or_v = type_map_or<T, Ts...>::value;

template <template <typename> class T, typename U>
constexpr bool type_not_v = type_not<T, U>::value;

template <typename ... Ts>
constexpr bool map_and_v = map_and<Ts...>::value;

template <typename ... Ts>
constexpr bool map_or_v = map_or<Ts...>::value;

template <typename T>
constexpr bool not_v = not_<T>::value;

template <typename ... Ts>
constexpr bool conjucation_v = conjucation<Ts...>::value;

template <typename ... Ts>
constexpr bool disjunction_v = disjunction<Ts...>::value;

template <typename T>
constexpr bool negation_v = negation<T>::value;

#endif              // HAVE_CPP14

PYCPP_END_NAMESPACE
