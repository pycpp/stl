//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if application is thread-safe in debug builds.
 *
 *  Check, at runtime, if non thread-safe code if they are executed in
 *  more than thread at a time. If the thread of execution changes,
 *  and the code is not thread-safe, abort program execution.
 *
 *  \synopsis
 *      template <bool ThreadSafe>
 *      struct checked_thread
 *      {
 *          checked_thread() noexcept;
 *          void check() const noexcept;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <cassert>
#include <thread>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <bool ThreadSafe>
struct checked_thread;

// Thread-safe class, disable
// thread ID checks.
template <>
struct checked_thread<true>
{
    checked_thread()
    noexcept
    {}

    // null-op
    void
    check()
    const noexcept
    {}
};

#ifdef NDEBUG               // NDEBUG

// No debugging support, disable
// thread ID checks.
template <>
struct checked_thread<false>
{
    checked_thread()
    noexcept
    {}

    // null-op
    void
    check()
    const noexcept
    {}
};

#else                       // !NDEBUG

// Enable thread ID checks.
template <>
struct checked_thread<false>
{
    checked_thread()
    noexcept:
        id(std::this_thread::get_id())
    {}

    void
    check()
    const noexcept
    {
        assert(
            id == std::this_thread::get_id() &&
            "Thread of execution does not match initial thread ID."
        );
    }

    std::thread::id id;
};

#endif                      // NDEBUG

PYCPP_END_NAMESPACE
