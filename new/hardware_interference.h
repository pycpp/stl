//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `hardware_*_interference_size` backport for C++11.
 *
 *  \synopsis
 *      static constexpr size_t hardware_destructive_interference_size = implementation-defined;
 *      static constexpr size_t hardware_constructive_interference_size = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/cache.h>
#include <pycpp/preprocessor/compiler.h>
#include <new>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// TODO(ahuszagh): Wishlist.
// GCC does not yet have the hardware interference constants yet.
#if defined(PYCPP_CPP17) && !defined(PYCPP_GCC)     // CPP17

using std::hardware_destructive_interference_size;
using std::hardware_constructive_interference_size;

#else                                               // <=CPP14

// FUNCTIONS
// ---------

static constexpr size_t hardware_destructive_interference_size = PYCPP_CACHELINE_SIZE;
static constexpr size_t hardware_constructive_interference_size = PYCPP_CACHELINE_SIZE;

#endif                                              // CPP17

PYCPP_END_NAMESPACE
