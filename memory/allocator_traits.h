//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Allocator traits to simplify memory allocation and construction.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      struct allocator_traits: std::allocator_traits<Allocator>
 *      {
 *          // TODO: implement
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

namespace stl_detail
{
// OBJECTS
// -------

template <typename Allocator>
struct allocator_traits: std::allocator_traits<Allocator>
{
// TODO: need custom allocator traits with....
// construct_backward
// construct_forward
};

}   /* stl_detail */

PYCPP_END_NAMESPACE
