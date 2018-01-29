//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Check if type uses allocator.
 *
 *  \synopsis
 *      template <typename T, typename Allocator>
 *      struct uses_allocator;
 */

#pragma once

#include <pycpp/config.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename T, typename Allocator>
struct uses_allocator: std::uses_allocator<T, Allocator>
{};

PYCPP_END_NAMESPACE
