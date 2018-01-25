//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `make_unique` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, typename ... Ts>
 *      unique_ptr<T> make_unique(Ts&&... ts);
 *
 *      template <typename T>
 *      unique_ptr<T> make_unique(size_t size);
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <memory>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP14)     // CPP14

using std::make_unique;

#else                       // <=CPP11

template <typename T, typename ... Ts>
std::unique_ptr<T>
make_unique(
    Ts&&... ts
)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(ts)...));
}

template <typename T>
std::unique_ptr<T>
make_unique(
    size_t size
)
{
    using type = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new type[size]);
}

#endif                      // HAVE_CPP14

PYCPP_END_NAMESPACE
