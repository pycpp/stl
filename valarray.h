//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <valarray> aliases.
 */

#pragma once

#include <pycpp/stl/utility.h>
#include <valarray>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::valarray;
using std::slice;
using std::slice_array;
using std::gslice;
using std::gslice_array;
using std::mask_array;
using std::indirect_array;

// Functions
using std::begin;
using std::end;
using std::abs;

// Exponential functions
using std::exp;
using std::log;
using std::log10;

// Power functions
using std::pow;
using std::sqrt;

// Trigonometric functions
using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan;
using std::atan2;

// Hyperbolic functions
using std::sinh;
using std::cosh;
using std::tanh;

PYCPP_END_NAMESPACE
