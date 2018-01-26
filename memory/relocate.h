//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Optimize item relocations similar to unitialized_move.
 *
 *  If the type is relocatable, use memcpy to directly copy items from
 *  an existing buffer to an uninitialized buffer, like
 *  `uninitialized_move`. If the type is not relocatable,
 *  directly use `uninitialized_move` to move into the buffer.
 *  Must be used with pointers (or class wrappers around raw pointers
 *  that maintain the original iterator order, IE, reverse_iterator
 *  does not count).
 *
 *  \synopsis
 *      template <typename P1, typename P2>
 *      void relocate(P1 src_first, P1 src_last, P2 dst_first);
 *
 *      template <typename P1, typename Size, typename P2>
 *      void relocate_n(P1 src_first, Size n, P2 dst_first);
 */

#include <pycpp/stl/memory/to_raw_pointer.h>
#include <pycpp/stl/memory/uninitialized.h>
#include <pycpp/stl/type_traits/is_relocatable.h>
#include <cstring>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

template <typename P1, typename Size, typename P2>
inline
void
relocate_n_impl(
    P1 src_first,
    Size n,
    P2 dst_first,
    std::true_type
)
{
    using src_traits = std::pointer_traits<P1>;
    using dst_traits = std::pointer_traits<P2>;
    using src_type = typename src_traits::element_type;
    using dst_type = typename dst_traits::element_type;

    char* src = reinterpret_cast<char*>(to_raw_pointer(src_first));
    char* dst = reinterpret_cast<char*>(to_raw_pointer(dst_first));
    Size bytes = n * sizeof(src_type);
    std::memmove(dst, src, bytes);
}


template <typename P1, typename Size, typename P2>
inline
void
relocate_n_impl(
    P1 src_first,
    Size n,
    P2 dst_first,
    std::false_type
)
{
    uninitialized_move_n(src_first, n, dst_first);
}

// FUNCTIONS
// ---------

template <typename P1, typename Size, typename P2>
inline
void
relocate_n(
    P1 src_first,
    Size n,
    P2 dst_first
)
{
    using value_type = typename std::pointer_traits<P1>::element_type;
    using relocatable = is_relocatable<P1>;
    relocate_n_impl(src_first, n, dst_first, relocatable());
}

template <typename P1, typename P2>
inline
void
relocate(
    P1 src_first,
    P1 src_last,
    P2 dst_first
)
{

    relocate_n(src_first, std::distance(src_first, src_last), dst_first);
}

PYCPP_END_NAMESPACE
