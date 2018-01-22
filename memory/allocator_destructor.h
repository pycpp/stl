//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Deleter for a unique_pointer wrapping an allocator.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      struct allocator_destructor
 *      {
 *          using allocator_type = Allocator;
 *          using pointer = implementation-defined;
 *          using size_type = implementation-defined;
 *
 *          allocator_destructor(allocator_type& alloc, size_type size) noexcept;
 *          void operator()(pointer p) noexcept;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

namespace stl_detail
{
// OBJECTS
// -------

template <typename Allocator>
class allocator_destructor
{
    using alloc_traits = allocator_traits<Allocator>;

public:
    using allocator_type = Allocator;
    using pointer = typename alloc_traits::pointer;
    using size_type = typename alloc_traits::size_type;

private:
    Allocator& alloc_;
    size_type size_;

public:
    allocator_destructor(
        allocator_type& alloc,
        size_type size
    )
    noexcept:
        alloc_(alloc),
        size_(size)
    {}

    void
    operator()(
        pointer p
    )
    noexcept
    {
        alloc_traits::deallocate(alloc_, p, size_);
    }
};

}   /* stl_detail */

PYCPP_END_NAMESPACE
