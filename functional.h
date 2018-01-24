//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <functional> aliases.
 */

#pragma once

#include <pycpp/stl/functional/bit_not.h>
#include <pycpp/stl/functional/boyer_moore_horspool_searcher.h>
#include <pycpp/stl/functional/boyer_moore_searcher.h>
#include <pycpp/stl/functional/default_searcher.h>
#include <pycpp/stl/functional/equal_to.h>
#include <pycpp/stl/functional/greater.h>
#include <pycpp/stl/functional/greater_equal.h>
#include <pycpp/stl/functional/hash.h>
#include <pycpp/stl/functional/invoke.h>
#include <pycpp/stl/functional/less.h>
#include <pycpp/stl/functional/less_equal.h>
#include <pycpp/stl/functional/not_equal_to.h>
#include <pycpp/stl/functional/not_fn.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

namespace placeholders
{
// Only bind the first 10
// Technically, this could lead to a compile-error.
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;
using std::placeholders::_6;
using std::placeholders::_7;
using std::placeholders::_8;
using std::placeholders::_9;
using std::placeholders::_10;

}   /* placeholders */

// Classes
using std::function;
using std::mem_fn;
using std::bad_function_call;
using std::is_bind_expression;
using std::is_placeholder;
using std::reference_wrapper;

// Functions
using std::bind;
using std::ref;
using std::cref;

// Functors
using std::plus;
using std::minus;
using std::multiplies;
using std::divides;
using std::modulus;
using std::negate;
using std::logical_and;
using std::logical_or;
using std::logical_not;
using std::bit_and;
using std::bit_or;
using std::bit_xor;
//using std::not_fn;
using std::unary_negate;
using std::binary_negate;
using std::not1;
using std::not2;
// using std::boyer_moore_searcher
// using std::boyer_moore_horspool_searcher

PYCPP_END_NAMESPACE
