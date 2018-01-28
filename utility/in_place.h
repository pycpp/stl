//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/nit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `in_place*` backports for C++11.
 *
 *  \synopsis
 *      struct in_place_t;
 *      constexpr in_place_t in_place {};
 *
 *      template <typename T>
 *      struct in_place_type_t;
 *
 *      template <std::size_t N>
 *      struct in_place_index_t;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr in_place_type_t<T> in_place_type {};
 *
 *      template <std::size_t N>
 *      constexpr in_place_index_t<N> in_place_index {};
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::in_place;
using std::in_place_t;
using std::in_place_type_t;
using std::in_place_type;
using std::in_place_index_t;
using std::in_place_index;

#else                               // <=CPP14

// OBJECTS
// -------

// IN PLACE T

struct in_place_t
{
    explicit in_place_t() = default;
};

constexpr in_place_t in_place {};

// IN PLACE TYPE T

template <typename T>
struct in_place_type_t
{
    explicit in_place_type_t() = default;
};

// IN PLACE INDEX T

template <std::size_t N>
struct in_place_index_t
{
    explicit in_place_index_t() = default;
};

#if defined(PYCPP_CPP14)            // CPP14

template <typename T>
constexpr in_place_type_t<T> in_place_type {};

template <std::size_t N>
constexpr in_place_index_t<N> in_place_index {};

#endif                              // CPP14

#endif                              // CPP17

PYCPP_END_NAMESPACE
