//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Smart pointer type-casts.
 *
 *  \synopsis
 *      template <typename T, typename U>
 *      shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) noexcept;
 *
 *      template <typename T, typename U>
 *      shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept;
 *
 *      template <typename T, typename U>
 *      shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) noexcept;
 *
 *      template <typename T, typename U>
 *      shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) noexcept;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;

#if defined(HAVE_CPP17)     // HAVE_CPP17

using std::reinterpret_pointer_cast;

#else                       // !HAVE_CPP17

template <typename T, typename U>
std::shared_ptr<T>
reinterpret_pointer_cast(
    const std::shared_ptr<U>& r
)
noexcept
{
    auto p = reinterpret_cast<typename std::shared_ptr<T>::element_type*>(r.get());
    return std::shared_ptr<T>(r, p);
}

#endif                      // HAVE_CPP17


// TODO: here...

PYCPP_END_NAMESPACE
