//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `aligned_alloc` backport for C++11.
 *
 *  \synopsis
 *      void* aligned_alloc(std::size_t alignment, std::size_t size);
 *      void* aligned_realloc(void* p, std::size_t alignment, std::size_t old_size, std::size_t new_size);
 *      void aligned_free(void* p);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <cstdlib>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

// `aligned_alloc` is not yet on GCC, fix when it becomes available.
#if PYCPP_GCC
    using ::aligned_alloc;
#else
    using std::aligned_alloc;
#endif

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

void* aligned_realloc(
   void *p,
   std::size_t alignment,
   std::size_t old_size,
   std::size_t new_size
);

void
aligned_free(
    void* p
);

PYCPP_END_NAMESPACE
