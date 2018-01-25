//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <condition_variable> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <condition_variable>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::condition_variable;
using std::condition_variable_any;
using std::cv_status;

// Functions
using std::notify_all_at_thread_exit;

PYCPP_END_NAMESPACE
