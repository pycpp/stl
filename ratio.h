//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <ratio> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/stdint.h>
#include <ratio>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::ratio;
using std::ratio_add;
using std::ratio_subtract;
using std::ratio_multiply;
using std::ratio_divide;
using std::ratio_equal;
using std::ratio_not_equal;
using std::ratio_less;
using std::ratio_less_equal;
using std::ratio_greater;
using std::ratio_greater_equal;
using std::atto;
using std::femto;
using std::pico;
using std::nano;
using std::micro;
using std::milli;
using std::centi;
using std::deci;
using std::deca;
using std::hecto;
using std::kilo;
using std::mega;
using std::giga;
using std::tera;
using std::peta;
using std::exa;

#if INTMAX_WIDTH > 64

using std::yocto;
using std::zepto;
using std::zetta;
using std::yotta;

#endif

PYCPP_END_NAMESPACE
