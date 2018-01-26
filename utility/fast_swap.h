//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Specialized swap.
 *
 *  Optimized swap algorithm that uses the following progression,
 *  implementing the first algorithm available:
 *      1. If the classes are empty, do nothing.
 *      1. Uses a member-function swap, if available.
 *      2. Uses a bitwise swap, if the type is relocatable.
 *      3. Uses `std::swap`.
 *
 *  `fast_swap` will not participate in overload resolution..
 *
 *  \synopsis
 *      template <class T>
 *      void fast_swap(T& x, T& y) noexcept;
 */

#pragma once

#include <pycpp/stl/type_traits/is_relocatable.h>
#include <pycpp/stl/type_traits/is_swappable.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// SWAP IMPL

// Null-op if the classes are empty..
template <
    typename T,
    typename std::enable_if<std::is_empty<T>::value>::type* = nullptr
>
inline
void
fast_swap_impl(
    T&,
    T&
)
noexcept
{}

// Declare swap function where specialized member function
// template exists.
template <
    typename T,
    typename std::enable_if<
        !std::is_empty<T>::value && is_member_swappable<T>::value
    >::type* = nullptr
>
inline
void
fast_swap_impl(
    T& x,
    T& y
)
noexcept(is_nothrow_member_swappable<T>::value)
{
    x.swap(y);
}


// Declare swap function when not member swappable but is
// relocatable, for a raw memcpy-based swap.
template <
    typename T,
    typename std::enable_if<
        !std::is_empty<T>::value && !is_member_swappable<T>::value && is_relocatable<T>::value
    >::type* = nullptr
>
inline
void
fast_swap_impl(
    T& x,
    T& y
)
noexcept
{
    using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
    storage_type tmp;
    std::memcpy(&tmp, &x, sizeof(T));
    std::memcpy(&x, &y, sizeof(T));
    std::memcpy(&y, &tmp, sizeof(T));
}

// Declare fallthrough swap function.
template <
    typename T,
    typename std::enable_if<
        !std::is_empty<T>::value && !is_member_swappable<T>::value && !is_relocatable<T>::value
    >::type* = nullptr
>
inline
void
fast_swap_impl(
    T& x,
    T& y
)
noexcept(noexcept(std::swap(x, y)))
{
    std::swap(x, y);
}

// SWAP

template <typename T>
inline
void
fast_swap(
    T& x,
    T& y
)
noexcept(noexcept(fast_swap_impl(x, y)))
{
    fast_swap_impl(x, y);
}

PYCPP_END_NAMESPACE
