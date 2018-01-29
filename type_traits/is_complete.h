//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Detect if type is complete.
 *
 *  \synopsis
 *      template <typename T>
 *      struct is_complete: implementation-defined
 *      {};
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_complete_v = implementation-defined;
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

template <typename T>
struct is_complete_impl
{
    template <typename U>
    static auto test(U*) -> std::integral_constant<bool, sizeof(U) == sizeof(U)>;
    static auto test(...) -> std::false_type;

    using type = decltype(test((T*)0));
};

template <typename T>
struct is_complete: is_complete_impl<T>::type
{};

#if defined(PYCPP_CPP14)

template <typename T>
constexpr bool is_complete_v = is_complete<T>::value;

#endif

PYCPP_END_NAMESPACE
