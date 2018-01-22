//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief C++17 backport of swappable detection.
 *
 *  Backport of `is_swappable_with`, `is_nothrow_swappable_with`,
 *  `is_swappable`, and `is_nothrow_swappable` for C++11.
 *
 *  \synopsis
 *      template <typename T1, typename T2>
 *      using is_swappable_with = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using is_nothrow_swappable_with = implementation-defined;
 *
 *      template <typename T>
 *      using is_swappable = is_swappable_with<T, T>;
 *
 *      template <typename T>
 *      using is_nothrow_swappable = is_nothrow_swappable_with<T, T>;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_swappable_with = implementation-defined;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_nothrow_swappable_with = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_swappable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_nothrow_swappable = implementation-defined;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_swappable_with_t = implementation-defined;
 *
 *      template <typename T1, typename T2, typename R = void>
 *      using enable_nothrow_swappable_with_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_swappable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_nothrow_swappable_t = implementation-defined;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T1, typename T2>
 *      constexpr bool is_swappable_with_v = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      constexpr bool is_nothrow_swappable_with_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_swappable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_nothrow_swappable_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <type_traits>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// TYPE

// Swappable (C++17 backport)
template <typename T1, typename T2>
struct is_swappable_with_impl
{
private:
    template <typename U1, typename U2, typename = decltype(swap(std::declval<U1&>(), std::declval<U2&>()))>
    static
    std::true_type
    test(int);

    template <typename U1, typename U2>
    static
    std::false_type
    test(...);

public:
    using type = decltype(test<T1, T2>(0));
};

template <typename T1, typename T2>
using is_swappable_with = typename is_swappable_with_impl<T1, T2>::type;

template <typename T1, typename T2, bool = is_swappable_with<T1, T2>::value>
struct is_nothrow_swappable_with
{
    static constexpr bool value = noexcept(swap(std::declval<T1&>(), std::declval<T2&>()));
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with<T1, T2, false>: std::false_type
{};

template <typename T>
using is_swappable = is_swappable_with<T, T>;

template <typename T>
using is_nothrow_swappable = is_nothrow_swappable_with<T, T>;

// ENABLE IF

template <typename T1, typename T2, typename R = void>
using enable_swappable_with = std::enable_if<is_swappable_with<T1, T2>::value, R>;

template <typename T1, typename T2, typename R = void>
using enable_nothrow_swappable_with = std::enable_if<is_nothrow_swappable_with<T1, T2>::value, R>;

template <typename T, typename R = void>
using enable_swappable = std::enable_if<is_swappable<T>::value, R>;

template <typename T, typename R = void>
using enable_nothrow_swappable = std::enable_if<is_nothrow_swappable<T>::value, R>;

template <typename T1, typename T2, typename R = void>
using enable_swappable_with_t = typename enable_swappable_with<T1, T2, R>::type;

template <typename T1, typename T2, typename R = void>
using enable_nothrow_swappable_with_t = typename enable_nothrow_swappable_with<T1, T2, R>::type;

template <typename T, typename R = void>
using enable_swappable_t = typename enable_swappable<T, R>::type;

template <typename T, typename R = void>
using enable_nothrow_swappable_t = typename enable_nothrow_swappable<T, R>::type;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T1, typename T2>
constexpr bool is_swappable_with_v = is_swappable_with<T1, T2>::value;

template <typename T1, typename T2>
constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T1, T2>::value;

template <typename T>
constexpr bool is_swappable_v = is_swappable<T>::value;

template <typename T>
constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

#endif              // HAVE_CPP14

PYCPP_END_NAMESPACE
