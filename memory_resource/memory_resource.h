//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Memory resource definition.
 *
 *  \synopsis
 *      struct memory_resource
 *      {
 *      public:
 *          virtual ~memory_resource();
 *
 *          void* allocate(size_t n, size_t alignment = implementation-defined);
 *
 *          void*
 *          reallocate(
 *              void* p,
 *              size_t old_size,
 *              size_t new_size,
 *              size_t n,
 *              size_t old_offset = 0,
 *              size_t new_offset = 0,
 *              size_t alignment = implementation-defined)
 *          );
 *
 *          void deallocate(void* p, size_t n, size_t alignment = implementation-defined);
 *          bool is_equal(const memory_resource&) const noexcept;
 *
 *      protected:
 *          virtual void* do_allocate(size_t n, size_t alignment) = 0;
 *
 *          virtual
 *          void*
 *          do_reallocate(
 *              void* p,
 *              size_t old_size,
 *              size_t new_size,
 *              size_t n,
 *              size_t old_offset,
 *              size_t new_offset,
 *              size_t alignment
 *          );
 *
 *          virtual void do_deallocate(void* p, size_t n, size_t alignment) = 0;
 *          virtual bool do_is_equal(const memory_resource&) const noexcept;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/cstddef/byte.h>
#include <pycpp/stl/memory/allocator_traits.h>
#include <atomic>
#include <limits>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

namespace pmr
{
// OBJECTS
// -------

// MEMORY RESOURCE

/**
 *  \brief Abstract base class for polymorphic resources.
 */
struct memory_resource
{
public:
    virtual
    ~memory_resource()
    {}

    // ALLOCATOR TRAITS
    void*
    allocate(
        size_t n,
        size_t alignment = alignof(std::max_align_t)
    )
    {
        return do_allocate(n, alignment);
    }

    void*
    reallocate(
        void* p,
        size_t old_size,
        size_t new_size,
        size_t n,
        size_t old_offset = 0,
        size_t new_offset = 0,
        size_t alignment = alignof(std::max_align_t)
    )
    {
        return do_reallocate(p, old_size, new_size, n, old_offset, new_offset, alignment);
    }

    void
    deallocate(
        void* p,
        size_t n,
        size_t alignment = alignof(std::max_align_t)
    )
    {
        do_deallocate(p, n, alignment);
    }

    bool
    is_equal(
        const memory_resource& x
    )
    const noexcept
    {
        return do_is_equal(x);
    }

protected:
    virtual
    void*
    do_allocate(
        size_t n,
        size_t alignment
    )
    = 0;

    // Warning: Only call this for relocatable types.
    // allocator_traits should force that behavior.
    virtual
    void*
    do_reallocate(
        void* p,
        size_t old_size,
        size_t new_size,
        size_t n,
        size_t old_offset,
        size_t new_offset,
        size_t alignment
    )
    {
        assert(n + old_offset <= old_size && "Buffer overflow.");
        assert(n + new_offset <= new_size && "Buffer overflow.");

        void* pout = do_allocate(new_size, alignment);
        byte* psrc = static_cast<byte*>(p);
        byte* pdest = static_cast<byte*>(pout);
        std::memcpy(pdest + new_offset, psrc + old_offset, n);
        do_deallocate(p, old_size, alignment);
        return pout;
    }

    virtual
    void do_deallocate(
        void* p,
        size_t n,
        size_t alignment
    )
    = 0;

    virtual
    bool
    do_is_equal(
        const memory_resource& x
    )
    const noexcept
    {
        return this == &x;
    }

private:
    static std::atomic<memory_resource*> default_resource_;

    friend
    memory_resource*
    set_default_resource(memory_resource* r)
    noexcept;

    friend
    memory_resource*
    get_default_resource()
    noexcept;
};

inline
bool
operator==(
    const memory_resource& x,
    const memory_resource& y
)
{
    return &x == &y || x.is_equal(y);
}


inline
bool
operator!=(
    const memory_resource& x,
    const memory_resource& y
)
{
    return !(x == y);
}

}   /* pmr */

PYCPP_END_NAMESPACE
