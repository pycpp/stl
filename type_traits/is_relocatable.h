//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Compile-time type detection to determine is a type is relocatable.
 *
 *  Cautiously assumes types are not relocatable, unless they are
 *  empty, trivially copyable or movable. Likewise, virtual types are
 *  considered not relocatable by default, since the virtual implementation
 *  is not standardized. Partial specializations for types may be provided,
 *  for increased performance when moving objects via a direct `memcpy`,
 *  rather than calling a move constructor and destructor for each
 *  object.
 *
 *  \synopsis
 *      template <typename T>
 *      struct is_relocatable;
 *
 *      struct is_virtual_relocatable: implementation-defined
 *      {};
 *
 *      template <typename T, typename R = void>
 *      using enable_relocatable = implementation-defined;
 *
 *      template <typename R = void>
 *      using enable_virtual_relocatable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_relocatable_t = implementation-defined;
 *
 *      template <typename R = void>
 *      using enable_virtual_relocatable_t = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_relocatable_v = implementation-defined;
 *
 *      constexpr bool is_virtual_relocatable_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/stl/type_traits/is_trivial.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// TYPE

// Relocatable (Proposed extension)
// `is_relocatable` should be specialized for any types if possible,
// since it allows raw bitwise copies.
// Inspired by Working Group paper P0023R0.
template <typename T>
struct is_relocatable: std::integral_constant<
        bool,
        // Empty classes rely on global, or no state,
        // and therefore can always be relocated.
        std::is_empty<T>::value ||
        // Trivially copyable classes are guaranteed to be copyable
        // via `memcpy`.
        is_trivially_copyable<T>::value ||
        // Trivially move-constructible classes should also satisfy
        // trivially copyable types.
        is_trivially_move_constructible<T>::value
    >
{};

// Virtual classes using virtual tables **are** relocatable, since
// they effectively contain a virtual table with a pointer to
// a static table containing the virtual function pointers.
// Almost every C++ virtual implementation uses vtables, however,
// it is not standardized, so we assume they are **not** copyable.
// If need be, specialize this to allow relocatable virtual classes,
// using book-keeping to track compilers using vtables.
struct is_virtual_relocatable: std::false_type
{};

// ENABLE IF

template <typename T, typename R = void>
using enable_relocatable = std::enable_if<
    is_relocatable<T>::value,
    R
>;

template <typename R = void>
using enable_virtual_relocatable = std::enable_if<
    is_virtual_relocatable::value,
    R
>;

template <typename T, typename R = void>
using enable_relocatable_t = typename enable_relocatable<T, R>::type;

template <typename R = void>
using enable_virtual_relocatable_t = typename enable_virtual_relocatable<R>::type;

#ifdef PYCPP_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_relocatable_v = is_relocatable<T>::value;

constexpr bool is_virtual_relocatable_v = is_virtual_relocatable::value;

#endif

PYCPP_END_NAMESPACE
