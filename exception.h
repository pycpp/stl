//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <exception> aliases.
 */

#pragma once

#include <pycpp/stl/exception/uncaught_exception.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::exception;
using std::nested_exception;
using std::bad_exception;

// Typedefs
using std::terminate_handler;
using std::exception_ptr;

// Functions
using std::make_exception_ptr;
using std::current_exception;
using std::rethrow_exception;
using std::throw_with_nested;
using std::rethrow_if_nested;
using std::terminate;
using std::get_terminate;
using std::set_terminate;

PYCPP_END_NAMESPACE
