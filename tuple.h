//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <tuple> aliases.
 */

#pragma once

#include <pycpp/stl/tuple/apply.h>
#include <pycpp/stl/tuple/make_from_tuple.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, typename Allocator>
struct uses_allocator;

// ALIAS
// -----

// Classes
// Inherit publicly to enable specializations
template <typename ... Ts>
struct tuple: std::tuple<Ts...>
{
    using std::tuple<Ts...>::tuple;
};

template <typename T>
struct tuple_size: std::tuple_size<T>
{};

template <size_t I, typename T>
struct tuple_element: std::tuple_element<I, T>
{};

template <typename ... Ts, typename Allocator>
struct uses_allocator<std::tuple<Ts...>, Allocator>: std::uses_allocator<std::tuple<Ts...>, Allocator>
{};

// Constants
using std::ignore;

// Functions
using std::make_tuple;
using std::tie;
using std::forward_as_tuple;
using std::tuple_cat;
using std::get;
using std::swap;

// CONVENIENCE
// -----------

template <std::size_t I, typename T>
using tuple_element_t = typename tuple_element<I, T>::type;

#ifdef HAVE_CPP14

template <typename T>
constexpr std::size_t tuple_size_v = tuple_size<T>::value;

#endif      // HAVE_CPP14

PYCPP_END_NAMESPACE
