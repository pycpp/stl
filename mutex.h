//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <mutex> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/mutex/dummy_mutex.h>
#include <mutex>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::mutex;
using std::timed_mutex;
using std::recursive_mutex;
using std::recursive_timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::defer_lock_t;
using std::try_to_lock_t;
using std::adopt_lock_t;
using std::defer_lock;
using std::try_to_lock;
using std::adopt_lock;
using std::once_flag;

// Functions
using std::try_lock;
using std::lock;
using std::call_once;
using std::swap;

PYCPP_END_NAMESPACE
