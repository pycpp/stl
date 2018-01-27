//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Mutex-like object without locking properties.
 *
 *  \synopsis
 *      struct dummy_mutex
 *      {
 *          using native_handle_type = std::nullptr_t;
 *
 *          constexpr dummy_mutex() noexcept;
 *          dummy_mutex(const dummy_mutex&) = delete;
 *          dummy_mutex& operator=(const dummy_mutex&) = delete;
 *
 *          void lock();
 *          bool try_lock();
 *          void unlock();
 *          native_handle_type native_handle();
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

struct dummy_mutex
{
    using native_handle_type = std::nullptr_t;

    constexpr
    dummy_mutex()
    noexcept
    {}

    dummy_mutex(const dummy_mutex&) = delete;
    dummy_mutex& operator=(const dummy_mutex&) = delete;

    void
    lock()
    {}

    bool
    try_lock()
    {
        return true;
    }

    void
    unlock()
    {}

    native_handle_type
    native_handle()
    {
        return nullptr;
    }
};

PYCPP_END_NAMESPACE
