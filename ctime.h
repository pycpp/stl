//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cmath> aliases.
 */

#pragma once

#include <pycpp/stl/ctime/timespec.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::clock_t;
using std::size_t;
using std::time_t;
using std::tm;

// Time manipulation
using std::clock;
using std::time;
using std::difftime;

// Format conversions
using std::ctime;
using std::asctime;
using std::strftime;
using std::gmtime;
using std::localtime;
using std::mktime;
// Typically not exported
//using std::wcsftime;

PYCPP_END_NAMESPACE
