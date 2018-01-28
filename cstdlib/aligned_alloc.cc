//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/preprocessor/compiler_traits.h>
#include <pycpp/stl/cstdlib/aligned_alloc.h>
#include <algorithm>
#include <cstring>
#if defined(PYCPP_MSVC)
#   include <malloc.h>
#endif

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// ALIGNED ALLOC

#if !defined(PYCPP_CPP17) && !defined(HAVE_ALIGNED_ALLOC)   // !CPP17 && !HAVE_ALIGNED_ALLOC

#if defined(PYCPP_WINDOWS)                                  // WINDOWS

PYCPP_MALLOC_ATTRIBUTE
void*
aligned_alloc(
    std::size_t alignment,
    std::size_t size
)
{
    // intentional argument inversion
    return _aligned_malloc(size, alignment);
}

#elif defined(HAVE_POSIX_MEMALIGN)                          // HAVE_POSIX_MEMALIGN

PYCPP_MALLOC_ATTRIBUTE
void*
aligned_alloc(
    std::size_t alignment,
    std::size_t size
)
{
    void *p;
    if (posix_memalign(&p, alignment, size) == 0) {
        return p;
    }
    return nullptr;
}

#else                                                       // MALLOC

PYCPP_MALLOC_ATTRIBUTE
void*
aligned_alloc(
    std::size_t,
    std::size_t size
)
{
    return std::malloc(size);
}

#endif                                                      // WINDOWS

#endif                                                      // !CPP17 && !HAVE_ALIGNED_ALLOC

// ALIGNED REALLOC

#if defined(PYCPP_WINDOWS)                                  // WINDOWS

void*
aligned_realloc(
   void *p,
   std::size_t alignment
   std::size_t /*old_size*/,
   std::size_t new_size
)
{
    // intentional argument inversion
    return _aligned_realloc(p, new_size, alignment);
}

#else                                                       // !WINDOWS
void*
aligned_realloc(
   void *p,
   std::size_t alignment,
   std::size_t old_size,
   std::size_t new_size
)
{
    // TODO(ahuszagh)
    // Wishlist. Optimize this routine to allow more efficiency
    // than an aligned_alloc + memcpy, allow grow, etc.

    void* pout = aligned_alloc(alignment, new_size);
    // If the allocator returns null, don't free the pointer and return NULL
    if (pout == nullptr) {
        return nullptr;
    }

    // no input memory, just return the allocated buffer.
    if (p == nullptr || old_size == 0) {
        return pout;
    }

    // copy bytes
    std::size_t count = std::min(old_size, new_size);
    std::memcpy(pout, p, count);

    // free old pointer
    aligned_free(p);

    return pout;
}

#endif                                                      // WINDOWS

// ALIGNED FREE

#if defined(PYCPP_CPP17) || defined(HAVE_ALIGNED_ALLOC)     // CPP17 || HAVE_ALIGNED_ALLOC

void
aligned_free(
    void* p
)
{
    std::free(p);
}

#elif defined(PYCPP_WINDOWS)                                // WINDOWS

void
aligned_free(
    void* p
)
{
    _aligned_free(p);
}

#elif defined(HAVE_POSIX_MEMALIGN)                          // HAVE_POSIX_MEMALIGN

void
aligned_free(
    void* p
)
{
    std::free(p);
}

#else                                                       // MALLOC

void
aligned_free(
    void* p
)
{
    std::free(p);
}

#endif                                                      // HAVE_ALIGNED_ALLOC

PYCPP_END_NAMESPACE
