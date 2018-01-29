//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `pointer_traits` backport for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      struct pointer_traits
 *      {
 *          using pointer = implementation-defined;
 *          using element_type = implementation-defined;
 *          using difference_type = implementation-defined;
 *          template <typename U> using rebind = implementation-defined;
 *
 *          static pointer pointer_to(element_type& r) noexcept;
 *          static element_type* to_address(pointer p) noexcept;
 *      };
 */

#pragma once

#include <pycpp/stl/memory/to_address.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename T>
struct pointer_traits: std::pointer_traits<T>
{
    using traits = std::pointer_traits<T>;
    using pointer = typename traits::pointer;
    using element_type = typename traits::element_type;

#if !defined(PYCPP_CPP20)

    static
    element_type*
    to_address(
        pointer p
    )
    noexcept
    {
        return PYSTD::to_address(p);
    }

#endif      // !CPP20
};

PYCPP_END_NAMESPACE
