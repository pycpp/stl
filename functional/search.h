//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `endian` backport for C++11.
 *
 *  \synopsis
 *      template <typename BinaryPredicate, typename ForwardIter1, typename ForwardIter2>
 *      constexpr std::pair<ForwardIter1, ForwardIter1>
 *      search(
 *          ForwardIter1 first1,
 *          ForwardIter1 last1,
 *          ForwardIter2 first2,
 *          ForwardIter2 last2,
 *          BinaryPredicate pred,
 *          std::forward_iterator_tag,
 *          std::forward_iterator_tag
 *      );
 *
 *      template <typename BinaryPredicate, typename RandomAccessIter1, typename RandomAccessIter2>
 *      constexpr std::pair<RandomAccessIter1, RandomAccessIter1>
 *      search(
 *          RandomAccessIter1 first1,
 *          RandomAccessIter1 last1,
 *          RandomAccessIter2 first2,
 *          RandomAccessIter2 last2,
 *          BinaryPredicate pred,
 *          std::random_access_iterator_tag,
 *          std::random_access_iterator_tag
 *      );
 */

#pragma once

#include <pycpp/preprocessor/compiler_traits.h>
#include <iterator>
#include <functional>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

template <typename BinaryPredicate, typename ForwardIter1, typename ForwardIter2>
PYCPP_CPP14_CONSTEXPR
std::pair<ForwardIter1, ForwardIter1>
search(
    ForwardIter1 first1,
    ForwardIter1 last1,
    ForwardIter2 first2,
    ForwardIter2 last2,
    BinaryPredicate pred,
    std::forward_iterator_tag,
    std::forward_iterator_tag
)
{
    if (first2 == last2) {
        // Everything matches an empty sequence
        return std::make_pair(first1, first1);
    }
    while (true) {
        // Find first element in sequence 1 that matchs *first2, with a mininum of loop checks
        while (true)
        {
            if (first1 == last1) {
                // return last1 if no element matches *first2
                return std::make_pair(last1, last1);
            }
            if (pred(*first1, *first2)) {
                break;
            }
            ++first1;
        }
        // *first1 matches *first2, now match elements after here
        ForwardIter1 m1 = first1;
        ForwardIter2 m2 = first2;
        while (true) {
            if (++m2 == last2) {
                // If pattern exhausted, first1 is the answer (works for 1 element pattern)
                return std::make_pair(first1, m1);
            }
            if (++m1 == last1) {
                // Otherwise if source exhaused, pattern not found
                return std::make_pair(last1, last1);
            }
            if (!pred(*m1, *m2)) {
                // if there is a mismatch, restart with a new first1
                ++first1;
                break;
            }
        }
    }
}

template <typename BinaryPredicate, typename RandomAccessIter1, typename RandomAccessIter2>
PYCPP_CPP14_CONSTEXPR
std::pair<RandomAccessIter1, RandomAccessIter1>
search(
    RandomAccessIter1 first1,
    RandomAccessIter1 last1,
    RandomAccessIter2 first2,
    RandomAccessIter2 last2,
    BinaryPredicate pred,
    std::random_access_iterator_tag,
    std::random_access_iterator_tag
)
{
    using diff1 = typename std::iterator_traits<RandomAccessIter1>::difference_type;
    using diff2 = typename std::iterator_traits<RandomAccessIter2>::difference_type;
    // Take advantage of knowing source and pattern lengths.  Stop short when source is smaller than pattern
    const diff2 len2 = last2 - first2;
    if (len2 == 0) {
        return std::make_pair(first1, first1);
    }
    const diff1 len1 = last1 - first1;
    if (len1 < len2) {
        return std::make_pair(last1, last1);
    }

     // Start of pattern match can't go beyond here
    const RandomAccessIter1 s = last1 - (len2 - 1);
    while (true) {
        while (true) {
            if (first1 == s) {
                return std::make_pair(last1, last1);
            }
            if (pred(*first1, *first2)) {
                break;
            }
            ++first1;
        }

        RandomAccessIter1 m1 = first1;
        RandomAccessIter2 m2 = first2;
         while (true) {
             if (++m2 == last2) {
                 return std::make_pair(first1, first1 + len2);
             }
             // no need to check range on m1 because s guarantees we have enough source
             ++m1;
             if (!pred(*m1, *m2)) {
                 ++first1;
                 break;
             }
         }
    }
}

PYCPP_END_NAMESPACE
