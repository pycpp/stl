//  :copyright: (c) 2014 Oliver Kowalke.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `integer_sequence` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, T ... Ints>
 *      class integer_sequence
 *      {
 *          using value_type = T;
 *          static constexpr std::size_t size() noexcept;
 *      };
 *
 *      template <std::size_t... Ints>
 *      using index_sequence = integer_sequence<std::size_t, Ints...>;
 *
 *      template <typename T, T N>
 *      using make_integer_sequence = implementation-defined;
 *
 *      template <std::size_t N>
 *      using make_index_sequence = make_integer_sequence<std::size_t, N>;
 *
 *      template <typename... T>
 *      using index_sequence_for = make_index_sequence<sizeof...(T)>;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <cstdlib>
#include <type_traits>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::integer_sequence;
using std::index_sequence;
using std::make_integer_sequence;
using std::make_index_sequence;
using std::index_sequence_for;

#else                               // <=CPP11

// OBJECTS
// -------

// INTEGER SEQUENCE

template <typename T, T... Is>
struct integer_sequence
{
    static_assert(
        std::is_integral<T>::value,
        "integer_sequence requires integral type"
    );

    using type = integer_sequence;
    using value_type = T;
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Is);
    }
};

// INDEX SEQUENCE

template <std::size_t... Is>
using index_sequence = integer_sequence<std::size_t, Is...>;

// MAKE INTEGER SEQUENCE

template <typename Seq1, typename Seq2>
struct concat_sequence;

template <typename T, T ... I1, T ... I2>
struct concat_sequence<
        integer_sequence<T, I1 ...>,
        integer_sequence<T, I2 ...>
    >:
    integer_sequence<T, I1..., (sizeof ...(I1)+I2) ...>
{};

template <typename T, T I, typename = void>
struct make_integer_sequence_impl: concat_sequence<
        typename make_integer_sequence_impl<T, I/2>::type,
        typename make_integer_sequence_impl<T, I-I/2>::type
    >
{};

template <typename T, T N>
struct make_integer_sequence_impl<T, N, typename std::enable_if<N == 0>::type>:
    integer_sequence<T>
{};

template <typename T, T N>
struct make_integer_sequence_impl<T, N, typename std::enable_if<N == 1>::type>:
    integer_sequence<T, 0>
{};

template <typename T, T I>
using make_integer_sequence = make_integer_sequence_impl<T, I>;

// MAKE INDEX SEQUENCE

template<std::size_t I>
using make_index_sequence = make_integer_sequence<std::size_t, I>;

// INDEX SEQUENCE FOR

template<typename ... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

#endif                              // CPP14

PYCPP_END_NAMESPACE
