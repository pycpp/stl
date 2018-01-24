//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `aligned_alloc` backport for C++11.
 *
 *  \synopsis
 *      void* aligned_alloc(std::size_t alignment, std::size_t size);
 *      void aligned_free(void* p);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <cstdlib>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::aligned_alloc;

#elif defined(HAVE_ALIGNED_ALLOC)   // HAVE_ALIGNED_ALLOC

using ::aligned_alloc;

#else                               // <=CPP14

void*
aligned_alloc(
    std::size_t alignment,
    std::size_t size
);

#endif                              // CPP17

// FUNCTIONS
// ---------

void
aligned_free(
    void* p
);

PYCPP_END_NAMESPACE
