//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Memory management utilities.
 */

#pragma once

#include <pycpp/stl/memory/allocator.h>
#include <pycpp/stl/memory/allocator_destructor.h>
#include <pycpp/stl/memory/allocator_traits.h>
#include <pycpp/stl/memory/destroy.h>
#include <pycpp/stl/memory/make_shared.h>
#include <pycpp/stl/memory/make_unique.h>
#include <pycpp/stl/memory/pointer_cast.h>
#include <pycpp/stl/memory/pointer_traits.h>
#include <pycpp/stl/memory/polymorphic_allocator.h>
#include <pycpp/stl/memory/shared_ptr.h>
#include <pycpp/stl/memory/to_address.h>
#include <pycpp/stl/memory/uninitialized.h>
#include <pycpp/stl/memory/uses_allocator.h>
#include <pycpp/stl/cstddef.h>
#include <pycpp/stl/cstdlib.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Smart pointers
using std::unique_ptr;
// TODO: remove
//using std::shared_ptr;
//using std::weak_ptr;
//using std::make_shared;
//using std::allocate_shared;
using std::get_deleter;
using std::swap;

// Helper classes
using std::owner_less;
//using std::enable_shared_from_this;
//using std::bad_weak_ptr;
using std::default_delete;

// Allocators
using std::allocator_arg_t;
using std::allocator_arg;

// Uninitialized storage
using std::uninitialized_copy;
using std::uninitialized_copy_n;
using std::uninitialized_fill;
using std::uninitialized_fill_n;
using std::raw_storage_iterator;
using std::get_temporary_buffer;
using std::return_temporary_buffer;

// Garbage Collector Support
using std::declare_reachable;
using std::undeclare_reachable;
using std::declare_no_pointers;
using std::undeclare_no_pointers;
using std::pointer_safety;
using std::get_pointer_safety;

// Atomics
using std::atomic_is_lock_free;
using std::atomic_load;
using std::atomic_load_explicit;
using std::atomic_store;
using std::atomic_store_explicit;
using std::atomic_exchange;
using std::atomic_exchange_explicit;
using std::atomic_compare_exchange_weak;
using std::atomic_compare_exchange_strong;
using std::atomic_compare_exchange_weak_explicit;
using std::atomic_compare_exchange_strong_explicit;

// Miscellaneous
using std::addressof;
using std::align;

// Create an alias for a universal allocator.
using byte_allocator = allocator<byte>;

// The following are from cstddef, so include them here
using std::nullptr_t;

// FORWARD
// -------

template <typename T>
struct hash;

// SPECIALIZATION
// --------------

template <typename T, typename Deleter>
struct hash<unique_ptr<T, Deleter>>
{
    using argument_type = unique_ptr<T, Deleter>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

template <typename T>
struct hash<shared_ptr<T>>
{
    using argument_type = shared_ptr<T>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

PYCPP_END_NAMESPACE
