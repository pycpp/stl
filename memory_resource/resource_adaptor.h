//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Resource adaptor for standard allocators.
 *
 *  \synopsis
*       template <typename Allocator>
*       struct resource_adaptor_impl;
*
*       template <typename Allocator>
*       using resource_adaptor = resource_adaptor_impl<
*           typename allocator_traits<Allocator>::template rebind_alloc<byte>
*       >;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/memory_resource/memory_resource.h>

PYCPP_BEGIN_NAMESPACE

namespace pmr
{
// HELPERS
// -------

inline
size_t
aligned_allocation_size(
    size_t n,
    size_t alignment
)
noexcept
{
    return (n + alignment - 1) & ~(alignment - 1);
}


inline
size_t
max_aligned_allocation_size(
    size_t align
)
{
    return std::numeric_limits<size_t>::max() - align;
}

// OBJECTS
// -------

/**
 *  \brief Adapts an STL allocator to a polymorphic resource.
 */
template <typename Allocator>
struct resource_adaptor_impl: memory_resource
{
    using allocator_type = Allocator;

    // CONSTRUCTORS
    resource_adaptor_impl() = default;
    resource_adaptor_impl(const resource_adaptor_impl&) = default;
    resource_adaptor_impl(resource_adaptor_impl&&) = default;
    resource_adaptor_impl& operator=(const resource_adaptor_impl&) = default;

    explicit
    resource_adaptor_impl(
        const allocator_type& alloc
    ):
        alloc_(alloc)
    {}

    explicit
    resource_adaptor_impl(
        allocator_type&& alloc
    ):
        alloc_(std::move(alloc))
    {}

    // Allocator traits
    allocator_type get_allocator() const
    {
        return alloc_;
    }

protected:
    // Memory traits
    virtual
    void*
    do_allocate(
        size_t n,
        size_t alignment
    )
    override
    {
        if (n > max_aligned_allocation_size(max_align)) {
            throw std::bad_alloc();
        }

        size_t s = aligned_allocation_size(n, max_align) / max_align;
        return static_cast<void*>(alloc_.allocate(s));
    }

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
    override
    {
        using value_type = typename std::allocator_traits<alloc>::value_type;

        if (new_size > max_aligned_allocation_size(max_align)) {
            throw std::bad_alloc();
        }

        value_type* pv = static_cast<value_type*>(p);
        size_t old_n = aligned_allocation_size(old_size, max_align) / max_align;
        size_t new_n = aligned_allocation_size(new_size, max_align) / max_align;
        value_type* pout = alloc_traits::reallocate(alloc_, pv, old_n, new_n, n, old_offset, new_offset);

        return static_cast<void*>(pout);
    }

    virtual
    void
    do_deallocate(
        void* p,
        size_t n,
        size_t alignment
    )
    override
    {
        using value_type = typename std::allocator_traits<alloc>::value_type;

        size_t s = aligned_allocation_size(n, max_align) / max_align;
        alloc_.deallocate(static_cast<value_type*>(p), s);
    }

    virtual
    bool
    do_is_equal(
        const memory_resource& rhs
    )
    const noexcept override
    {
        const resource_adaptor_impl* p = dynamic_cast<const resource_adaptor_impl*>(&rhs);
        return p ? alloc_ == p->alloc_ : false;
    }

private:
    static constexpr size_t max_align = alignof(std::max_align_t);
    using traits_type = allocator_traits<Allocator>;
    using storage = typename std::aligned_storage<max_align, max_align>::type;
    using alloc = typename traits_type::template rebind_alloc<storage>;
    using alloc_traits = allocator_traits<alloc>;
    static_assert(std::is_same<typename traits_type::value_type, byte>::value, "");

    alloc alloc_;
};

// ALIAS
// -----

template <typename Allocator>
using resource_adaptor = resource_adaptor_impl<
    typename allocator_traits<Allocator>::template rebind_alloc<byte>
>;

}   /* pmr */

PYCPP_END_NAMESPACE
