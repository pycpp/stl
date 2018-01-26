//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect if type is allocator-constructible.
 *
 *  \synopsis
 *      template <typename Allocator, typename T, typename ... Ts>
 *      struct has_construct: implementation-defined
 *      {};
 *
 *      template <typename Allocator, typename T>
 *      struct has_memcpy_construct: implementation-defined
 *      {};
 *
 *      template <typename Allocator, typename T, typename R = void>
 *      using enable_memcpy_construct = implementation-defined;
 *
 *      template <typename Allocator, typename T, typename R = void>
 *      using enable_memcpy_construct_t = implementation-defined;
 *
 *      #ifdef HAVE_CPP14
 *
 *      template <typename T>
 *      constexpr bool has_construct_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool has_memcpy_construct_v = implementation-defined;
 *
 *      #endif              // HAVE_CPP14
 */

#pragma once

#include <pycpp/stl/type_traits/is_relocatable.h>
#include <memory>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

template <typename Allocator, typename T, typename ... Ts>
decltype(
    std::declval<Allocator>().construct(std::declval<T*>(), std::declval<Ts>()...),
    std::true_type()
)
has_construct_test(
    Allocator&& alloc,
    T* p,
    Ts&&... ts
);

template <typename Allocator, typename Pointer, typename ... Ts>
std::false_type
has_construct_test(
    const Allocator& alloc,
    Pointer&& p,
    Ts&&... ts
);

// OBJECTS
// -------

template <typename Allocator, typename T, typename ... Ts>
struct has_construct:
    std::integral_constant<bool,
        std::is_same<
            decltype(has_construct_test(
                std::declval<Allocator>(),
                std::declval<T*>(),
                std::declval<Ts>()...
            )),
            std::true_type
        >::value
    >
{};


template <typename Allocator, typename T>
struct has_memcpy_construct:
    std::integral_constant<
        bool,
        (
            is_relocatable<T>::value &&
            std::is_same<typename std::allocator_traits<Allocator>::pointer, T*>::value
        )
    >
{};

template <typename Allocator, typename T, typename R = void>
using enable_memcpy_construct = std::enable_if<
    has_memcpy_construct<Allocator, T>::value,
    R
>;

template <typename Allocator, typename T, typename R = void>
using enable_memcpy_construct_t = typename enable_memcpy_construct<Allocator, T, R>::type;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_construct_v = has_construct<T>::value;

template <typename T>
constexpr bool has_memcpy_construct_v = has_memcpy_construct<T>::value;

#endif              // HAVE_CPP14

PYCPP_END_NAMESPACE
