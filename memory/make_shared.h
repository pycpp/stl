//  :copyright: (c) 2007, 2008, 2012 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `make_shared` utility functions.
 *
 *  Deleters for the shared_ptr helpers `make_shared` and
 *  `allocate_shared`.
 *
 *  \synopsis
 *      template <typename T>
 *      class sp_ms_deleter
 *      {
 *      public:
 *          sp_ms_deleter() noexcept;
 *          template <typename A> explicit sp_ms_deleter(const A&) noexcept;
 *          sp_ms_deleter(const sp_ms_deleter&) noexcept;
 *          ~sp_ms_deleter() noexcept;
 *          void operator()(T*) noexcept;
 *          static void operator_fn(T*) noexcept;
 *          void* address() noexcept;
 *          void set_initialized() noexcept;
 *      };
 *
 *      template <typename T, typename Allocator>
 *      class sp_as_deleter
 *      {
 *      public:
 *          explicit sp_as_deleter(const Allocator& alloc) noexcept;
 *          sp_as_deleter(const sp_as_deleter& r) noexcept;
 *          ~sp_as_deleter() noexcept;
 *          void operator()(T*);
 *          static void operator_fn(T*) noexcept;
 *          void* address() noexcept;
 *          void set_initialized() noexcept;
 *      };
 */

#pragma once

#include <pycpp/stl/memory/allocator.h>
#include <pycpp/stl/memory/allocator_traits.h>
#include <pycpp/stl/memory/shared_count.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// SP MS DELETER

template <typename T>
class sp_ms_deleter
{
public:

    sp_ms_deleter()
    noexcept:
        initialized_(false)
    {}

    template <typename Allocator>
    explicit
    sp_ms_deleter(
        const Allocator&
    )
    noexcept:
        initialized_(false)
    {}

    // optimization: do not copy storage_
    sp_ms_deleter(
        const sp_ms_deleter&
    )
    noexcept:
        initialized_(false)
    {}

    ~sp_ms_deleter()
    noexcept
    {
        destroy();
    }

    void
    operator()(
        T*
    )
    noexcept
    {
        destroy();
    }

    static
    void
    operator_fn(T*)
    noexcept
    {}

    void*
    address()
    noexcept
    {
        return static_cast<void*>(&storage_);
    }

    void
    set_initialized()
    noexcept
    {
        initialized_ = true;
    }

private:
    using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    bool initialized_;
    storage_type storage_;

    void
    destroy()
    noexcept
    {
        if (initialized_) {
            T* p = static_cast<T*>(address());
            p->~T();
            initialized_ = false;
        }
    }
};

// SP AS DELETER

template <typename T, typename Allocator>
class sp_as_deleter
{
public:
    explicit
    sp_as_deleter(
        const Allocator& alloc
    )
    noexcept:
        alloc_(alloc),
        initialized_(false)
    {}

    // optimization: do not copy storage_
    sp_as_deleter(
        const sp_as_deleter& r
    )
    noexcept:
        alloc_(r.alloc_),
        initialized_(false)
    {}

    ~sp_as_deleter()
    noexcept
    {
        destroy();
    }

    void
    operator()(
        T*
    )
    noexcept
    {
        destroy();
    }

    static
    void
    operator_fn(T*)
    noexcept
    {}

    void*
    address()
    noexcept
    {
        return static_cast<void*>(&storage_);
    }

    void
    set_initialized()
    noexcept
    {
        initialized_ = true;
    }

private:
    using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    storage_type storage_;
    Allocator alloc_;
    bool initialized_;

    void
    destroy()
    noexcept
    {
        if (initialized_) {
            T* p = static_cast<T*>(address());
            allocator_traits<Allocator>::destroy(alloc_, p);
            initialized_ = false;
        }
    }
};

PYCPP_END_NAMESPACE
