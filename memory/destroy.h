//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `destroy_*` backports for C++11.
 *
 *  \synopsis
 *      template <typename T>
 *      void destroy_at(T* p);
 *
 *      template <typename ForwardIter>
 *      void destroy(ForwardIter first, ForwardIter last);
 *
 *      template <typename ForwardIter, typename Size>
 *      ForwardIter destroy_n(ForwardIter first, Size n);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <cassert>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)    // CPP17

using std::destroy_at;
using std::destroy;
using std::destroy_n;

#else                       // !CPP17

// FUNCTIONS
// ---------

template <typename T>
inline
void
destroy_at(
    T* p
)
{
    assert(p && "null pointer given to destroy_at");
    p->~T();
}

template <typename ForwardIter>
inline
void
destroy(
    ForwardIter first,
    ForwardIter last
)
{
    for (; first != last; ++first) {
        destroy_at(std::addressof(*first));
    }
}

template <typename ForwardIter, typename Size>
inline
ForwardIter
destroy_n(
    ForwardIter first,
    Size n
)
{
    for (; n > 0; (void)++first, --n) {
        destroy_at(std::addressof(*first));
    }
    return first;
}

#endif                      // CPP17

PYCPP_END_NAMESPACE
