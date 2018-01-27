//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief General purpose allocator.
 *
 *  \synopsis
 *      template <typename T>
 *      struct allocator;
 */

#pragma once

#include <pycpp/stl/memory/allocator_traits.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// TODO: change to a specialized allocator
// Need to make the allocator dependent on various sizes
template <typename T>
struct allocator: std::allocator<T>
{
    using std::allocator<T>::allocator;
};

// FUNCTIONS
// ---------

/**
 *  \brief `new` analog for a custom allocator.
 */
template <typename T, typename Allocator, typename ... Ts>
inline
T*
allocate_and_construct(
    const Allocator& allocator,
    Ts&&... ts
)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    // the allocator should throw if there's no memory available
    allocator_type alloc(allocator);
    T* t = traits_type::allocate(alloc, 1);
    traits_type::construct(alloc, t, std::forward<Ts>(ts)...);
    return t;
}


/**
 *  \brief `delete` analog for a custom allocator.
 */
template <typename T, typename Allocator>
inline
void
destroy_and_deallocate(
    const Allocator& allocator,
    T* t,
    size_t n = 1
)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    allocator_type alloc(allocator);
    traits_type::destroy(alloc, t);
    traits_type::deallocate(alloc, t, n);
}

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable;

template <typename T>
struct is_relocatable<allocator<T>>: std::true_type
{};

PYCPP_END_NAMESPACE
