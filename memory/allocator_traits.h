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
 *
 *          template <typename Pointer>
 *          static void construct_forward(allocator_type& alloc, Pointer begin1, Pointer end1, Pointer& begin2);
 *
 *          template <typename Iter, typename Pointer>
 *          static void construct_range_forward(allocator_type& alloc, Iter begin1, Iter end1, Pointer& begin2)
 *
 *          template <typename Pointer>
 *          static void construct_backward(allocator_type& alloc, Pointer begin1, Pointer end1, Pointer& end2)
 *      };
 */

#pragma once

#include <pycpp/stl/memory/has_construct.h>
#include <pycpp/stl/memory/to_raw_pointer.h>
#include <pycpp/stl/type_traits.h>
#include <cstring>
#include <memory>
#include <utility>

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

    // Reallocate

    // Overload if class provides specialized reallocate
    // Only use for relocatable types, since the type might
    // not be relocatable, in which case the types
    // still must be moved into position.
    template <typename T = value_type, typename A = allocator_type>
    enable_if_t<has_reallocate<A>::value && is_relocatable<T>::value, pointer>
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
    enable_if_t<!is_relocatable<T>::value, pointer>
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

    // Construct forward
    template <typename Pointer>
    static
    void
    construct_forward(
        allocator_type& alloc,
        Pointer begin1,
        Pointer end1,
        Pointer& begin2
    )
    {
        for (; begin1 != end1; ++begin1, ++begin2) {
            traits::construct(alloc, to_raw_pointer(begin2), std::move_if_noexcept(*begin1));
        }
    }

    template <typename T>
    static
    enable_memcpy_construct<allocator_type, T>
    construct_forward(
        allocator_type&,
        T* begin1,
        T* end1,
        T*& begin2
    )
    {
        std::ptrdiff_t n = end1 - begin1;
        if (n > 0) {
            std::memcpy(begin2, begin1, n * sizeof(T));
            begin2 += n;
        }
    }

    // Construct range forward
    template <typename Iter, typename Pointer>
    static
    void
    construct_range_forward(
        allocator_type& alloc,
        Iter begin1,
        Iter end1,
        Pointer& begin2
    )
    {
        for (; begin1 != end1; ++begin1, (void) ++begin2) {
            traits::construct(alloc, to_raw_pointer(begin2), *begin1);
        }
    }

    template <typename T>
    static
    enable_memcpy_construct<allocator_type, T>
    construct_range_forward(
        allocator_type&,
        T* begin1,
        T* end1,
        T*& begin2
    )
    {
        using value_type = typename std::remove_const<T>::type;
        std::ptrdiff_t n = end1 - begin1;
        if (n > 0) {
            std::memcpy(const_cast<value_type*>(begin2), begin1, n * sizeof(T));
            begin2 += n;
        }
    }

    // Construct backward
    template <typename Pointer>
    static
    void
    construct_backward(
        allocator_type& alloc,
        Pointer begin1,
        Pointer end1,
        Pointer& end2
    )
    {
        while (end1 != begin1) {
            traits::construct(alloc, to_raw_pointer(end2-1), std::move_if_noexcept(*--end1));
            --end2;
        }
    }

    template <typename T>
    static
    enable_memcpy_construct<allocator_type, T>
    construct_backward(allocator_type&, T* begin1, T* end1, T*& end2)
    {
        std::ptrdiff_t n = end1 - begin1;
        end2 -= n;
        if (n > 0) {
            std::memcpy(end2, begin1, n * sizeof(T));
        }
    }
};

PYCPP_END_NAMESPACE
