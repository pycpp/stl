//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <chrono> aliases.
 */

#pragma once

#include <pycpp/stl/ratio.h>
#include <pycpp/stl/chrono/abs.h>
#include <pycpp/stl/chrono/ceil.h>
#include <pycpp/stl/chrono/floor.h>
#include <pycpp/stl/chrono/round.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Specializations
using std::common_type;

namespace chrono
{
// Classes
using std::chrono::duration;
using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::treat_as_floating_point;
using std::chrono::duration_values;

// Convenience typedefs
using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;

// Functions
using std::chrono::duration_cast;
using std::chrono::time_point_cast;

}   /* chrono */

PYCPP_END_NAMESPACE
