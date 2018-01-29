//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Specialized swap for allocators.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      void swap_allocator(Allocator& a1, Allocator& a2) noexcept;
 */

#pragma once

#include <pycpp/stl/memory/allocator_traits.h>
#include <pycpp/stl/utility/fast_swap.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

template <typename Allocator>
inline
void
swap_allocator(
    Allocator& a1,
    Allocator& a2
)
noexcept
{
    using propagate = typename allocator_traits<Allocator>::propagate_on_container_swap;
    swap_allocator(a1, a2, std::integral_constant<bool, propagate::value>());
}

template <typename Allocator>
void swap_allocator(
    Allocator& a1,
    Allocator& a2,
    std::true_type
)
noexcept
{
    fast_swap(a1, a2);
}

template <typename Allocator>
inline
void
swap_allocator(
    Allocator&,
    Allocator&,
    std::false_type
)
noexcept
{}

PYCPP_END_NAMESPACE
