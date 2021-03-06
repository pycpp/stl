//  :copyright: (c) 2011-2012 Andrzej Krzemienski.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <utility> aliases.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/cstdlib.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/utility/as_const.h>
#include <pycpp/stl/utility/chars.h>
#include <pycpp/stl/utility/exchange.h>
#include <pycpp/stl/utility/fast_swap.h>
#include <pycpp/stl/utility/in_place.h>
#include <pycpp/stl/utility/integer_sequence.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename ... Ts>
struct tuple;

template <typename T>
struct tuple_size;

template <std::size_t I, typename T>
struct tuple_element;

// ALIAS
// -----

namespace rel_ops
{

using std::rel_ops::operator!=;
using std::rel_ops::operator>;
using std::rel_ops::operator<=;
using std::rel_ops::operator>=;

}   /* rel_ops */

// Functions
using std::swap;
using std::forward;
using std::move;
using std::move_if_noexcept;
using std::declval;
using std::make_pair;
using std::get;

// Classes
using std::pair;

// Helpers
using std::piecewise_construct_t;
using std::piecewise_construct;

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable;

template <typename T1, typename T2>
struct is_relocatable<pair<T1, T2>>: std::integral_constant<
        bool,
        is_relocatable<T1>::value && is_relocatable<T2>::value
    >
{};

PYCPP_END_NAMESPACE
