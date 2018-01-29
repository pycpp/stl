//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <new> aliases.
 */

#pragma once

#include <pycpp/stl/exception.h>
#include <pycpp/stl/new/hardware_interference.h>
#include <pycpp/stl/new/launder.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

//Functions
using std::get_new_handler;
using std::set_new_handler;

// Classes
using std::bad_alloc;
using std::bad_array_new_length;

//Types
using std::new_handler;

// Objects
using std::nothrow_t;
using std::nothrow;

PYCPP_END_NAMESPACE
