//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
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

#if defined(HAVE_CPP17)             // CPP17

using std::hardware_destructive_interference_size;
using std::hardware_constructive_interference_size;

#else                               // <=CPP14

// FUNCTIONS
// ---------

static constexpr size_t hardware_destructive_interference_size = CACHELINE_SIZE;
static constexpr size_t hardware_constructive_interference_size = CACHELINE_SIZE;

#endif                              // CPP17

PYCPP_END_NAMESPACE
