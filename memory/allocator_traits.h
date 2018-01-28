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
 *          using is_always_equal = implementation-defined;
 *
 *          pointer reallocate(allocator_type& allocator, pointer ptr, size_type old_size, size_type new_size, size_type count, size_type old_offset = 0, size_type new_offset = 0);
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
#include <pycpp/stl/type_traits/has_is_always_equal.h>
#include <pycpp/stl/type_traits/has_reallocate.h>
#include <pycpp/stl/type_traits/is_relocatable.h>
#include <cassert>
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
    using is_always_equal = typename std::conditional<
        has_is_always_equal<Allocator>::value,
        typename Allocator::is_always_equal,
        typename std::is_empty<Allocator>::type
    >::type;

    // Unsafe Reallocate
    // Reallocation functions without checks for type safety.
    // Do not use directly, only if wishing to call directly as a fallback
    // in a custom allocator.

    // Use an optimized memcpy to relocate the data from the old to
    // new buffers.
    static
    pointer
    reallocate_relocate(
        allocator_type& alloc,
        pointer ptr,
        size_type old_size,
        size_type new_size,
        size_type count,
        size_type old_offset = 0,
        size_type new_offset = 0
    )
    {
        assert(count + old_offset <= old_size && "Buffer overflow.");
        assert(count + new_offset <= new_size && "Buffer overflow.");

        pointer p = alloc.allocate(new_size);
        value_type* psrc = to_raw_pointer(ptr);
        value_type* pdest = to_raw_pointer(p);
        std::memcpy(pdest + new_offset, psrc + old_offset, count * sizeof(value_type));
        alloc.deallocate(ptr, old_size);
        return p;
    }

    // Use the slow route, create a new buffer and use `traits::construct`
    // with move semantics to move all items to the new buffer.
    static
    pointer
    reallocate_move(
        allocator_type& alloc,
        pointer ptr,
        size_type old_size,
        size_type new_size,
        size_type count,
        size_type old_offset = 0,
        size_type new_offset = 0
    )
    {
        assert(count + old_offset <= old_size && "Buffer overflow.");
        assert(count + new_offset <= new_size && "Buffer overflow.");

        pointer p = alloc.allocate(new_size);
        pointer npoff = p + new_offset;
        pointer opoff = ptr + old_offset;
        // use `construct` to construct-in-place
        // Don't use a raw `std::move`, since that move assigns into
        // uninitialized memory.
        for (size_t i = 0; i < count; ++i) {
            value_type& src = opoff[i];
            value_type* dst = &npoff[i];
            traits::construct(alloc, dst, std::move(src));
        }
        alloc.deallocate(ptr, old_size);
        return p;
    }

    // Reallocate

    // Reallocate allocates the size similarly to `allocate` and
    // `deallocate`, it uses the number of objects and not the
    // number of bytes.
    // \param alloc             Allocator
    // \param old_size          Old size of buffer (in objects, not bytes)
    // \param new_size          New size of buffer (in objects, not bytes)
    // \param count             Number of objects to move from old to new
    // \param old_offset        Object offset for copying from old-buffer
    // \param new_offset        Object offset for copying into old-buffer

    // Overload if class provides specialized reallocate
    // Only use for relocatable types, since the type might
    // not be relocatable, in which case the types
    // still must be moved into position.
    template <typename T = value_type, typename A = allocator_type>
    static
    typename std::enable_if<has_reallocate<A>::value && is_relocatable<T>::value, pointer>::type
    reallocate(
        allocator_type& alloc,
        pointer ptr,
        size_type old_size,
        size_type new_size,
        size_type count,
        size_type old_offset = 0,
        size_type new_offset = 0
    )
    {
        assert(count + old_offset <= old_size && "Buffer overflow.");
        assert(count + new_offset <= new_size && "Buffer overflow.");

        return alloc.reallocate(ptr, old_size, new_size, count, old_offset, new_offset);
    }

    // Overload if class does not provide specialized reallocate
    // and can be trivially moved as bytes.
    template <typename T = value_type, typename A = allocator_type>
    static
    typename std::enable_if<!has_reallocate<A>::value && is_relocatable<T>::value, pointer>::type
    reallocate(
        allocator_type& alloc,
        pointer ptr,
        size_type old_size,
        size_type new_size,
        size_type count,
        size_type old_offset = 0,
        size_type new_offset = 0
    )
    {
        return reallocate_relocate(alloc, ptr, old_size, new_size, count, old_offset, new_offset);
    }

    // Overload if class does not provide specialized reallocate
    // and cannot be trivially moved as bytes.
    template <typename T = value_type, typename A = allocator_type>
    static
    typename std::enable_if<!is_relocatable<T>::value, pointer>::type
    reallocate(
        allocator_type& alloc,
        pointer ptr,
        size_type old_size,
        size_type new_size,
        size_type count,
        size_type old_offset = 0,
        size_type new_offset = 0
    )
    {
        return reallocate_move(alloc, ptr, old_size, new_size, count, old_offset, new_offset);
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
    enable_memcpy_construct_t<allocator_type, T>
    construct_forward(
        allocator_type&,
        T* begin1,
        T* end1,
        T*& begin2
    )
    {
        std::ptrdiff_t n = end1 - begin1;
        if (n > 0) {
            std::memmove(begin2, begin1, n * sizeof(T));
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
    enable_memcpy_construct_t<allocator_type, T>
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
            std::memmove(const_cast<value_type*>(begin2), begin1, n * sizeof(T));
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
    enable_memcpy_construct_t<allocator_type, T>
    construct_backward(
        allocator_type&,
        T* begin1,
        T* end1,
        T*& end2
    )
    {
        std::ptrdiff_t n = end1 - begin1;
        end2 -= n;
        if (n > 0) {
            std::memmove(end2, begin1, n * sizeof(T));
        }
    }
};

PYCPP_END_NAMESPACE
