//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `chars_format`, `to_chars`, `from_chars` backport for C++11.
 *
 *  \synopsis
 *      enum class chars_format
 *      {
 *          scientific = implementation-defined,
 *          fixed = implementation-defined,
 *          hex = implementation-defined,
 *          general = fixed | scientific
 *      };
 // TODO: implement...
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::chars_format;
using std::to_chars;
using std::from_chars;

#else                               // <=CPP14

// ENUMS
// -----

// TODO: no good working implementation in Clang yet.
// TODO: here's the working group paper:
//      http://open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0067r4.html
//enum class chars_format
//{
//
//    scientific = implementation-defined,
//    fixed = implementation-defined,
//    hex = implementation-defined,
//    general = fixed | scientific
//};


#endif                              // CPP17

PYCPP_END_NAMESPACE



