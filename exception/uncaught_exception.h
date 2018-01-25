//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `uncaught_exception` backport for C++11.
 *
 *  \synopsis
 *      bool uncaught_exception() noexcept;
 *      int uncaught_exceptions() noexcept;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <exception>

PYCPP_BEGIN_NAMESPACE

// ALIASES
// -------

using std::uncaught_exception;

#if defined(HAVE_CPP17)             // CPP17

using std::uncaught_exceptions;

#else                               // <=CPP14

// FUNCTIONS
// ---------

int
uncaught_exceptions()
noexcept;

#endif                              // CPP17

PYCPP_END_NAMESPACE
