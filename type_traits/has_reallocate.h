//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect if allocator supports reallocating memory.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      struct has_reallocate;
 *
 *      template <typename T, typename R = void>
 *      using enable_reallocate = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_reallocate_t = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool has_reallocate_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <memory>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// TYPE

// Check if the allocator has `reallocate`, an extension.
template <typename T>
class has_reallocate_impl
{
protected:
    // Reallocate has a function declaration as follows:
    // pointer
    // reallocate(
    //      pointer ptr,
    //      size_type old_size,
    //      size_type new_size,
    //      size_type count,
    //      size_type old_offset = 0,
    //      size_type new_offset = 0
    // )

    template <typename C>
    static
    char
    &test(
        decltype(
            std::declval<C>().reallocate(
                std::declval<typename std::allocator_traits<C>::pointer>(),
                std::declval<typename std::allocator_traits<C>::size_type>(),
                std::declval<typename std::allocator_traits<C>::size_type>(),
                std::declval<typename std::allocator_traits<C>::size_type>(),
                std::declval<typename std::allocator_traits<C>::size_type>(),
                std::declval<typename std::allocator_traits<C>::size_type>()
            )
        )
    );

    template <typename C>
    static
    long
    &test(...);

public:
    enum {
        value = sizeof(test<T>(0)) == sizeof(char)
    };
};

template <typename T>
struct has_reallocate: std::integral_constant<bool, has_reallocate_impl<T>::value>
{};

// ENABLE IF

template <typename T, typename R = void>
using enable_reallocate = std::enable_if<
    has_reallocate<T>::value,
    R
>;

template <typename T, typename R = void>
using enable_reallocate_t = typename enable_reallocate<T, R>::type;

#ifdef PYCPP_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_reallocate_v = has_reallocate<T>::value;

#endif

PYCPP_END_NAMESPACE
