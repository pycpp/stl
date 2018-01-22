//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Convert wrapped pointer to raw pointer.
 *
 *  \synopsis
 *      template <typename T>
 *      typename std::pointer_traits<Pointer>::element_type* to_raw_pointer(T t);
 */

#pragma once

#include <pycpp/config.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


template <typename T>
inline constexpr
T*
to_raw_pointer(
    T* p
)
noexcept
{
    return p;
}

template <typename Pointer>
inline
typename std::pointer_traits<Pointer>::element_type*
to_raw_pointer(
    Pointer p
)
noexcept
{
    return to_raw_pointer(p.operator->());
}

PYCPP_END_NAMESPACE
