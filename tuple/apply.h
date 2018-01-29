//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `apply` backport for C++11.
 *
 *  \synopsis
 *      template <typename F, typename Tuple>
 *      constexpr implementation-defined apply(F&& f, Tuple&& t);
 */

#pragma once

#include <pycpp/stl/functional/invoke.h>
#include <pycpp/stl/utility/integer_sequence.h>
#include <tuple>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::apply;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename F, typename Tuple, std::size_t... I>
constexpr
auto
apply_impl(
    F&& f,
    Tuple&& t,
    index_sequence<I...>
)
-> decltype(invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...))
{
    return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}

template <typename F, typename Tuple>
constexpr
auto
apply(
    F&& f,
    Tuple&& t
)
-> decltype(apply_impl(
    std::forward<F>(f),
    std::forward<Tuple>(t),
    make_index_sequence<
        std::tuple_size<typename std::remove_reference<Tuple>::type>::value
    >{}
))
{
    using tuple_type = typename std::remove_reference<Tuple>::type;
    using sequence = make_index_sequence<std::tuple_size<tuple_type>::value>;

    return apply_impl(
        std::forward<F>(f),
        std::forward<Tuple>(t),
        sequence{}
    );
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
