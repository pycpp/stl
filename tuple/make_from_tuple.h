//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `make_from_tuple` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, typename Tuple>
 *      constexpr T make_from_tuple(Tuple&& t);
 */

#pragma once

#include <pycpp/stl/utility/integer_sequence.h>
#include <tuple>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::make_from_tuple;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename T, typename Tuple, std::size_t... I>
inline constexpr
T
make_from_tuple_impl(
    Tuple&& t,
    index_sequence<I...>
)
{
    return T(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename T, typename Tuple>
inline constexpr
T
make_from_tuple(
    Tuple&& t
)
{
    using tuple_type = typename std::remove_reference<Tuple>::type;
    using sequence = make_index_sequence<std::tuple_size<tuple_type>::value>;

    return make_from_tuple_impl<T>(
        std::forward<Tuple>(t),
        sequence{}
    );
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
