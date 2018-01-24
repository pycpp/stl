//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `endian` backport for C++11.
 *
 *  \synopsis
// TODO: implement
 */

#pragma once

#include <pycpp/stl/functional/equal_to.h>
#include <pycpp/stl/functional/search.h>
#include <iterator>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::default_searcher;

#else                               // <=CPP14

// OBJECTS
// -------

template<typename ForwardIterator, typename BinaryPredicate = equal_to<>>
class default_searcher
{
public:
    default_searcher(
        ForwardIterator f,
        ForwardIterator l,
        BinaryPredicate p = BinaryPredicate()
    ):
        first_(f),
        last_(l),
        pred_(p)
    {}

    template <typename ForwardIterator2>
    std::pair<ForwardIterator2, ForwardIterator2>
    operator()(
        ForwardIterator2 f,
        ForwardIterator2 l
    )
    const
    {
        return search(f, l, first_, last_, pred_,
            typename std::iterator_traits<ForwardIterator>::iterator_category(),
            typename std::iterator_traits<ForwardIterator2>::iterator_category()
        );
    }

private:
    ForwardIterator first_;
    ForwardIterator last_;
    BinaryPredicate pred_;
};

#endif                              // CPP17

PYCPP_END_NAMESPACE
