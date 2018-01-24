//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect if type is allocator-constructible.
 *
 *  \synopsis
 *      template <typename Allocator, typename Pointer, typename ... Ts>
 *      struct has_construct: implementation-defined
 *      {};
 *
 *      template <typename Allocator, typename T>
 *      struct has_memcpy_construct: implementation-defined
 *      {};
 *
 *      template <typename Allocator, typename T, typename R = void>
 *      using enable_memcpy_construct = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>
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

template <typename Allocator, typename Pointer, typename ... Ts>
struct has_construct:
    std::integral_constant<bool,
        std::is_same<
            decltype(has_construct_test(
                std::declval<Allocator>(),
                std::declval<Pointer>(),
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
        std::is_trivially_move_constructible<T>::value &&
        (
            std::is_same<Allocator, std::allocator<T> >::value ||
            has_construct<Allocator, T*, T>::value
        )
    >
{};

template <typename Allocator, typename T, typename R = void>
using enable_memcpy_construct = typename std::enable_if<
    has_memcpy_construct<Allocator, T>::value,
    R
>::type;

PYCPP_END_NAMESPACE
