//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/cstdlib/aligned_alloc.h>
#if defined(HAVE_MSVC)
#   include <malloc.h>
#endif

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// ALIGNED ALLOC

#if !defined(HAVE_CPP17) && !defined(HAVE_ALIGNED_ALLOC)    // !CPP17 && !HAVE_ALIGNED_ALLOC

#if defined(HAVE_MSVC)                                      // HAVE_MSVC

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

#endif                                                      // HAVE_MSVC

#endif                                                      // !CPP17 && !HAVE_ALIGNED_ALLOC

// ALIGNED FREE

#if defined(HAVE_CPP17) || defined(HAVE_ALIGNED_ALLOC)      // HAVE_CPP17 || HAVE_ALIGNED_ALLOC

void
aligned_free(
    void* p
)
{
    std::free(p);
}

#elif defined(HAVE_MSVC)                                    // HAVE_MSVC

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
