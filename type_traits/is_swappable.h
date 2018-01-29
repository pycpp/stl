//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
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
 *      template <typename T>
 *      using is_member_swappable = implementation-defined;
 *
 *      template <typename T>
 *      using is_nothrow_member_swappable = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using enable_swappable_with = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using enable_nothrow_swappable_with = implementation-defined;
 *
 *      template <typename T>
 *      using enable_swappable = implementation-defined;
 *
 *      template <typename T>
 *      using enable_nothrow_swappable = implementation-defined;
 *
 *      template <typename T>
 *      using enable_member_swappable = implementation-defined;
 *
 *      template <typename T>
 *      using enable_nothrow_member_swappable = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using enable_swappable_with_t = implementation-defined;
 *
 *      template <typename T1, typename T2>
 *      using enable_nothrow_swappable_with_t = implementation-defined;
 *
 *      template <typename T>
 *      using enable_swappable_t = implementation-defined;
 *
 *      template <typename T>
 *      using enable_nothrow_swappable_t = implementation-defined;
 *
 *      template <typename T>
 *      using enable_member_swappable_t = implementation-defined;
 *
 *      template <typename T>
 *      using enable_nothrow_member_swappable_t = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
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
 *      template <typename T>
 *      constexpr bool is_member_swappable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_nothrow_member_swappable_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/type_traits/has_member_function.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// TYPE

// Swappable (C++17 backport)

// IS SWAPPABLE WITH

template <typename T1, typename T2>
struct is_swappable_with_adl
{
    // Try ADL
    template <
        typename L,
        typename R,
        typename = decltype(swap(std::declval<L>(), std::declval<R>()))
    >
    static
    std::true_type
    test_swap(int);

    template <typename, typename>
    static
    std::false_type
    test_swap(...);

    using swap1 = decltype(test_swap<T1, T2>(0));
    using swap2 = decltype(test_swap<T2, T1>(0));

    static constexpr bool value = swap1::value && swap2::value;
};


template <typename T1, typename T2>
struct is_swappable_with_std
{
    // Try ADL
    template <
        typename L,
        typename R,
        typename = decltype(std::swap(std::declval<L>(), std::declval<R>()))
    >
    static
    std::true_type
    test_swap(int);

    template <typename, typename>
    static
    std::false_type
    test_swap(...);

    using swap1 = decltype(test_swap<T1, T2>(0));
    using swap2 = decltype(test_swap<T2, T1>(0));

    static constexpr bool value = swap1::value && swap2::value;
};


template <
    typename T1,
    typename T2 = T1,
    bool NotVoid = !std::is_void<T1>::value && !std::is_void<T2>::value
>
struct is_swappable_with_impl
{
    // check ADL or STD lookup, either works.
    static constexpr bool value = (
        is_swappable_with_adl<T1, T2>::value ||
        is_swappable_with_std<T1, T2>::value
    );
};

template <typename T1, typename T2>
struct is_swappable_with_impl<T1, T2, false>: std::false_type
{};

template <typename T1, typename T2>
struct is_swappable_with: std::integral_constant<
        bool,
        is_swappable_with_impl<T1, T2>::value
    >
{};

// IS NOTHROW SWAPPABLE WITH

template <
    typename T1,
    typename T2,
    bool AdlSwappable = is_swappable_with_adl<T1, T2>::value
>
struct is_nothrow_swappable_with_adl
{
    static constexpr bool value = (
        noexcept(swap(std::declval<T1>(), std::declval<T2>())) &&
        noexcept(swap(std::declval<T2>(), std::declval<T1>()))
    );
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with_adl<T1, T2, false>: std::false_type
{};

template <
    typename T1,
    typename T2,
    bool StdSwappable = is_swappable_with_std<T1, T2>::value
>
struct is_nothrow_swappable_with_std
{
    static constexpr bool value = (
        noexcept(std::swap(std::declval<T1>(), std::declval<T2>())) &&
        noexcept(std::swap(std::declval<T2>(), std::declval<T1>()))
    );
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with_std<T1, T2, false>: std::false_type
{};

template <typename T1, typename T2, bool = is_swappable_with<T1, T2>::value>
struct is_nothrow_swappable_with_impl
{
    // check ADL or STD lookup, either works.
    static constexpr bool value = (
        is_nothrow_swappable_with_adl<T1, T2>::value ||
        is_nothrow_swappable_with_std<T1, T2>::value
    );
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with_impl<T1, T2, false>: std::false_type
{};

template <typename T1, typename T2>
struct is_nothrow_swappable_with: std::integral_constant<
        bool,
        is_nothrow_swappable_with_impl<T1, T2>::value
    >
{};

// IS SWAPPABLE

template <typename T>
struct is_swappable: is_swappable_with<T&, T&>
{};

// IS NOTHROW SWAPPABLE

template <typename T>
struct is_nothrow_swappable: is_nothrow_swappable_with<T&, T&>
{};

// IS MEMBER SWAPPABLE

PYCPP_HAS_MEMBER_FUNCTION(swap, is_member_swappable_impl, void (C::*)(C&));

template <typename T>
struct is_member_swappable: is_member_swappable_impl<T>
{};

// IS NOTHROW MEMBER SWAPPABLE

template <typename T, bool IsSwappable = is_member_swappable<T>::value>
struct is_nothrow_member_swappable_impl
{
    static constexpr bool value = noexcept(std::declval<T&>().swap(std::declval<T&>()));
};

template <typename T>
struct is_nothrow_member_swappable_impl<T, false>
{
    static constexpr bool value = false;
};

template <typename T>
struct is_nothrow_member_swappable: is_nothrow_member_swappable_impl<T>
{};

// ENABLE IF

template <typename T1, typename T2>
using enable_swappable_with = std::enable_if<is_swappable_with<T1, T2>::value>;

template <typename T1, typename T2>
using enable_nothrow_swappable_with = std::enable_if<is_nothrow_swappable_with<T1, T2>::value>;

template <typename T>
using enable_swappable = std::enable_if<is_swappable<T>::value>;

template <typename T>
using enable_nothrow_swappable = std::enable_if<is_nothrow_swappable<T>::value>;

template <typename T>
using enable_member_swappable = std::enable_if<is_member_swappable<T>::value>;

template <typename T>
using enable_nothrow_member_swappable = std::enable_if<is_nothrow_member_swappable<T>::value>;

template <typename T1, typename T2>
using enable_swappable_with_t = typename enable_swappable_with<T1, T2>::type;

template <typename T1, typename T2>
using enable_nothrow_swappable_with_t = typename enable_nothrow_swappable_with<T1, T2>::type;

template <typename T>
using enable_swappable_t = typename enable_swappable<T>::type;

template <typename T>
using enable_nothrow_swappable_t = typename enable_nothrow_swappable<T>::type;

template <typename T>
using enable_member_swappable_t = typename enable_member_swappable<T>::type;

template <typename T>
using enable_nothrow_member_swappable_t = typename enable_nothrow_member_swappable<T>::type;

#ifdef PYCPP_CPP14

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

template <typename T>
constexpr bool is_member_swappable_v = is_member_swappable<T>::value;

template <typename T>
constexpr bool is_nothrow_member_swappable_v = is_nothrow_member_swappable<T>::value;

#endif

PYCPP_END_NAMESPACE
