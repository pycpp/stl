//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <mutex> aliases.
 */

#pragma once

#include <pycpp/stl/thread/checked_thread.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::thread;

// Functions
using std::swap;
using std::hash;

namespace this_thread
{

using std::this_thread::yield;
using std::this_thread::get_id;
using std::this_thread::sleep_for;
using std::this_thread::sleep_until;

}   /* this_thread */

PYCPP_END_NAMESPACE
