//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Allocator traits to simplify memory allocation and construction.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      struct allocator_traits: std::allocator_traits<Allocator>
 *      {
 *          using allocator_type = Allocator;
 *
 *          pointer reallocate(allocator_type& allocator, pointer ptr, size_type old_size, size_type new_size);
 *      };
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename Allocator>
struct allocator_traits: std::allocator_traits<Allocator>
{
    using allocator_type = Allocator;
    using traits = std::allocator_traits<allocator_type>;
    using typename traits::value_type;
    using typename traits::pointer;
    using typename traits::size_type;

    // Overload if class provides specialized reallocate
    template <typename T = value_type, typename A = allocator_type>
    enable_reallocate_t<A, pointer>
    reallocate(
        allocator_type& allocator,
        pointer ptr,
        size_type old_size,
        size_type new_size
    )
    {
        return allocator.reallocate(ptr, old_size, new_size);
    }

    // Overload if class does not provide specialized reallocate
    // and can be trivially moved as bytes.
    template <typename T = value_type, typename A = allocator_type>
    enable_if_t<!has_reallocate<A>::value && is_relocatable<T>::value, pointer>
    reallocate(
        allocator_type& allocator,
        pointer ptr,
        size_type old_size,
        size_type new_size
    )
    {
        pointer p = allocator.allocate(new_size);
        memcpy((void*) p, (void*) ptr, old_size * sizeof(value_type));
        allocator.deallocate(ptr, old_size);
        return p;
    }

    // Overload if class does not provide specialized reallocate
    // and cannot be trivially moved as bytes.
    template <typename T = value_type, typename A = allocator_type>
    enable_if_t<!has_reallocate<A>::value && !is_relocatable<T>::value, pointer>
    reallocate(
        allocator_type& allocator,
        pointer ptr,
        size_type old_size,
        size_type new_size
    )
    {
        pointer p = allocator.allocate(new_size);
        // use placement new to construct-in-place
        // Don't use `std::move`, since that move assigns into
        // uninitialized memory.
        for (size_t i = 0; i < old_size; ++i) {
            T& src = ptr[i];
            T* dst = &p[i];
            new (static_cast<void*>(dst)) T(std::move(src));
        }
        allocator.deallocate(ptr, old_size);
        return p;
    }

// TODO: need custom allocator traits with....
// construct_backward
// construct_forward
};

PYCPP_END_NAMESPACE
