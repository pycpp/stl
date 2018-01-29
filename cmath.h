//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cmath> aliases.
 */

#pragma once

#include <pycpp/stl/cmath/assoc_laguerre.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::float_t;
using std::double_t;

// Basic operations
using std::abs;
using std::fabs;
using std::fmod;
using std::remainder;
using std::remquo;
using std::fma;
using std::fmax;
using std::fmin;
using std::fdim;
using std::nan;
using std::nanf;
using std::nanl;

// Exponential functions
using std::exp;
using std::exp2;
using std::expm1;
using std::log;
using std::log10;
using std::log2;
using std::log1p;

// Power functions
using std::pow;
using std::sqrt;
using std::cbrt;
using std::hypot;

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
using std::asinh;
using std::acosh;
using std::atanh;

// Error and gamma functions
using std::erf;
using std::erfc;
using std::tgamma;
using std::lgamma;

// Nearest integer floating point operations
using std::ceil;
using std::floor;
using std::trunc;
using std::round;
using std::lround;
using std::llround;
using std::nearbyint;
using std::rint;
using std::lrint;
using std::llrint;

// Floating point manipulation functions
using std::frexp;
using std::ldexp;
using std::modf;
using std::scalbn;
using std::scalbln;
using std::ilogb;
using std::logb;
using std::nextafter;
using std::nexttoward;
using std::copysign;

// Classification and comparison
using std::fpclassify;
using std::isfinite;
using std::isinf;
using std::isnan;
using std::isnormal;
using std::signbit;
using std::isgreater;
using std::isgreaterequal;
using std::isless;
using std::islessequal;
using std::islessgreater;
using std::isunordered;

PYCPP_END_NAMESPACE
