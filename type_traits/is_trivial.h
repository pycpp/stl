//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type traits for trivial constructors and assignment operators.
 *
 *  Reasonable default implementations of `is_trivially_*` type traits
 *  for compilers with partial C++11 support.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_trivially_copyable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_default_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_copy_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_move_constructible = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using is_trivially_assignable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_copy_assignable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_move_assignable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copyable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_constructible = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_default_constructible = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copy_constructible = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_move_constructible = implementation-defined;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_trivially_assignable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copy_assignable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_move_assignable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copyable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_constructible_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_default_constructible_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copy_constructible_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_move_constructible_t = implementation-defined;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_trivially_assignable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_copy_assignable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_trivially_move_assignable_t = implementation-defined;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_trivially_copyable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_constructible_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_default_constructible_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_copy_constructible_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_move_constructible_v = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      constexpr bool is_trivially_assignable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_copy_assignable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_trivially_move_assignable_v = implementation-defined;
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

// `is_trivially_destructible` is supported by GCC early on in
// the 4.x series, and therefore does not have to be wrapped here.

#if defined(CPP11_PARTIAL_TYPE_TRAITS)      // CPP11_PARTIAL_TYPE_TRAITS

// Guess all trivially constructible/assignable types are
// `is_trivial`.

template <typename T>
using is_trivially_copyable = std::is_trivial<T>;

template <typename T>
using is_trivially_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_default_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_copy_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_move_constructible = std::is_trivial<T>;

template <typename T1, typename T2>
using is_trivially_assignable = std::integral_constant<
    bool,
    std::is_trivial<T1>::value,
    std::is_trivial<T2>::value
>;

template <typename T>
using is_trivially_copy_assignable = std::is_trivial<T>;

template <typename T>
using is_trivially_move_assignable = std::is_trivial<T>;

#else                                       // !CPP11_PARTIAL_TYPE_TRAITS

using std::is_trivially_copyable;
using std::is_trivially_constructible;
using std::is_trivially_default_constructible;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
using std::is_trivially_assignable;
using std::is_trivially_copy_assignable;
using std::is_trivially_move_assignable;

#endif                                      // CPP11_PARTIAL_TYPE_TRAITS

// SFINAE
// ------

// ENABLE IF

template <typename T, typename R = void>
using enable_trivially_copyable = std::enable_if<is_trivially_copyable<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_constructible = std::enable_if<is_trivially_constructible<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_default_constructible = std::enable_if<is_trivially_default_constructible<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_copy_constructible = std::enable_if<is_trivially_copy_constructible<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_move_constructible = std::enable_if<is_trivially_move_constructible<T>::value, R>;

template <typename T1, typename T2, typename R = void>
using enable_trivially_assignable = std::enable_if<is_trivially_assignable<T1, T2>::value, R>;

template <typename T, typename R = void>
using enable_trivially_copy_assignable = std::enable_if<is_trivially_copy_assignable<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_move_assignable = std::enable_if<is_trivially_move_assignable<T>::value, R>;

template <typename T, typename R = void>
using enable_trivially_copyable_t = typename enable_trivially_copyable<T, R>::type;

template <typename T, typename R = void>
using enable_trivially_constructible_t = typename enable_trivially_constructible<T, R>::type;

template <typename T, typename R = void>
using enable_trivially_default_constructible_t = typename enable_trivially_default_constructible<T, R>::type;

template <typename T, typename R = void>
using enable_trivially_copy_constructible_t = typename enable_trivially_copy_constructible<T, R>::type;

template <typename T, typename R = void>
using enable_trivially_move_constructible_t = typename enable_trivially_move_constructible<T, R>::type;

template <typename T1, typename T2, typename R = void>
using enable_trivially_assignable_t = typename enable_trivially_assignable<T1, T2, R>::type;

template <typename T, typename R = void>
using enable_trivially_copy_assignable_t = typename enable_trivially_copy_assignable<T, R>::type;

template <typename T, typename R = void>
using enable_trivially_move_assignable_t = typename enable_trivially_move_assignable<T, R>::type;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

template <typename T>
constexpr bool is_trivially_constructible_v = is_trivially_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

template <typename T>
constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

template <typename T1, typename T2>
constexpr bool is_trivially_assignable_v = is_trivially_assignable<T1, T2>::value;

template <typename T>
constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

template <typename T>
constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

#endif              // HAVE_CPP14

PYCPP_END_NAMESPACE
