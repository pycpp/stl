//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
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

#include <pycpp/stl/memory/allocator_traits.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <
    typename Allocator,
    std::size_t Size = 1,
    bool IsAlwaysEqual = allocator_traits<Allocator>::is_always_equal::value
>
class allocator_destructor;

// Always equal
template <typename Allocator, std::size_t Size>
class allocator_destructor<Allocator, Size, true>: Allocator
{
public:
    using allocator_type = Allocator;
    using alloc_traits = allocator_traits<allocator_type>;
    using pointer = typename alloc_traits::pointer;
    using size_type = typename alloc_traits::size_type;

    allocator_destructor(
        allocator_type&
    )
    noexcept
    {}

    void
    operator()(
        pointer p
    )
    noexcept
    {
        allocator_type alloc;
        alloc_traits::deallocate(alloc, p, Size);
    }

    allocator_type&
    get_allocator()
    {
        return *this;
    }
};

// Not always equal
template <typename Allocator, std::size_t Size>
class allocator_destructor<Allocator, Size, false>
{
public:
    using allocator_type = Allocator;
    using alloc_traits = allocator_traits<allocator_type>;
    using pointer = typename alloc_traits::pointer;
    using size_type = typename alloc_traits::size_type;

public:
    allocator_destructor(
        allocator_type& alloc
    )
    noexcept:
        alloc_(alloc)
    {}

    void
    operator()(
        pointer p
    )
    noexcept
    {
        alloc_traits::deallocate(alloc_, p, Size);
    }

    allocator_type&
    get_allocator()
    {
        return alloc_;
    }

private:
    Allocator& alloc_;
};

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable;

template <typename T, std::size_t Size>
struct is_relocatable<allocator_destructor<T, Size, true>>: std::true_type
{};

// References are not relocatable
template <typename T, std::size_t Size>
struct is_relocatable<allocator_destructor<T, Size, false>>: std::false_type
{};

PYCPP_END_NAMESPACE
