//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
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
 *
 *      template <typename T, typename U, typename TypeTraits>
 *      intrusive_ptr<T, TypeTraits> static_pointer_cast(const intrusive_ptr<U, TypeTraits>& r) noexcept;
 *
 *      template <typename T, typename U, typename TypeTraits>
 *      intrusive_ptr<T, TypeTraits> dynamic_pointer_cast(const intrusive_ptr<U, TypeTraits>& r) noexcept;
 *
 *      template <typename T, typename U, typename TypeTraits>
 *      intrusive_ptr<T, TypeTraits> const_pointer_cast(const intrusive_ptr<U, TypeTraits>& r) noexcept;
 *
 *      template <typename T, typename U, typename TypeTraits>
 *      intrusive_ptr<T, TypeTraits> reinterpret_pointer_cast(const intrusive_ptr<U, TypeTraits>& r) noexcept;
 */

#pragma once

#include <pycpp/stl/memory/intrusive_ptr.h>
#include <pycpp/stl/memory/shared_ptr.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// SHARED PTR

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

// INTRUSIVE PTR

template <typename T, typename U, typename TraitsType>
inline
intrusive_ptr<T, TraitsType>
static_pointer_cast(
    const intrusive_ptr<U, TraitsType>& r
)
noexcept
{
    auto p = static_cast<typename intrusive_ptr<T, TraitsType>::element_type*>(r.get());
    return intrusive_ptr<T, TraitsType>(p);
}


template <typename T, typename U, typename TraitsType>
inline
intrusive_ptr<T, TraitsType>
dynamic_pointer_cast(
    const intrusive_ptr<U, TraitsType>& r
)
noexcept
{
    // if the pointer is null, fine.
    auto p = dynamic_cast<typename intrusive_ptr<T, TraitsType>::element_type*>(r.get());
    return intrusive_ptr<T, TraitsType>(p);
}


template <typename T, typename U, typename TraitsType>
inline
intrusive_ptr<T, TraitsType>
const_pointer_cast(
    const intrusive_ptr<U, TraitsType>& r
)
noexcept
{
    auto p = const_cast<typename intrusive_ptr<T, TraitsType>::element_type*>(r.get());
    return intrusive_ptr<T, TraitsType>(p);
}


template <typename T, typename U, typename TraitsType>
inline
intrusive_ptr<T, TraitsType>
reinterpret_pointer_cast(
    const intrusive_ptr<U, TraitsType>& r
)
noexcept
{
    auto p = reinterpret_cast<typename intrusive_ptr<T, TraitsType>::element_type*>(r.get());
    return intrusive_ptr<T, TraitsType>(p);
}

PYCPP_END_NAMESPACE
