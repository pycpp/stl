//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <iomanip> aliases.
 */

#pragma once

#include <pycpp/stl/iomanip/quoted.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::resetiosflags;
using std::setiosflags;
using std::setbase;
using std::setfill;
using std::setprecision;
using std::setw;
using std::get_money;
using std::put_money;
using std::get_time;
using std::put_time;

PYCPP_END_NAMESPACE
