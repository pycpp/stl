//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Smart pointer type-casts.
 *
 *  \synopsis
 *      template <typename T, typename U, bool ThreadSafe>
 *      shared_ptr<T, ThreadSafe> static_pointer_cast(const shared_ptr<U, ThreadSafe>& r) noexcept;
 *
 *      template <typename T, typename U, bool ThreadSafe>
 *      shared_ptr<T, ThreadSafe> dynamic_pointer_cast(const shared_ptr<U, ThreadSafe>& r) noexcept;
 *
 *      template <typename T, typename U, bool ThreadSafe>
 *      shared_ptr<T, ThreadSafe> const_pointer_cast(const shared_ptr<U, ThreadSafe>& r) noexcept;
 *
 *      template <typename T, typename U, bool ThreadSafe>
 *      shared_ptr<T, ThreadSafe> reinterpret_pointer_cast(const shared_ptr<U, ThreadSafe>& r) noexcept;
 */

#pragma once

#include <pycpp/stl/memory/shared_ptr.h>

PYCPP_BEGIN_NAMESPACE

// TODO: implement them all, since I use my own shared_ptr type

// FUNCTIONS
// ---------

template <typename T, typename U, bool ThreadSafe>
inline
shared_ptr<T, ThreadSafe>
static_pointer_cast(
    const shared_ptr<U, ThreadSafe>& r
)
noexcept
{
    auto p = static_cast<typename shared_ptr<T, ThreadSafe>::element_type*>(r.get());
    return shared_ptr<T, ThreadSafe>(r, p);
}


template <typename T, typename U, bool ThreadSafe>
inline
shared_ptr<T, ThreadSafe>
dynamic_pointer_cast(
    const shared_ptr<U, ThreadSafe>& r
)
noexcept
{
    if (auto p = dynamic_cast<typename shared_ptr<T, ThreadSafe>::element_type*>(r.get())) {
        return shared_ptr<T, ThreadSafe>(r, p);
    } else {
        return shared_ptr<T, ThreadSafe>();
    }
}


template <typename T, typename U, bool ThreadSafe>
inline
shared_ptr<T, ThreadSafe>
const_pointer_cast(
    const shared_ptr<U, ThreadSafe>& r
)
noexcept
{
    auto p = const_cast<typename shared_ptr<T, ThreadSafe>::element_type*>(r.get());
    return shared_ptr<T, ThreadSafe>(r, p);
}


template <typename T, typename U, bool ThreadSafe>
inline
shared_ptr<T, ThreadSafe>
reinterpret_pointer_cast(
    const shared_ptr<U, ThreadSafe>& r
)
noexcept
{
    auto p = reinterpret_cast<typename shared_ptr<T, ThreadSafe>::element_type*>(r.get());
    return shared_ptr<T, ThreadSafe>(r, p);
}

PYCPP_END_NAMESPACE
