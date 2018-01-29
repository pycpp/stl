//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
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

#if defined(PYCPP_CPP17)            // CPP17

// None of the major compilers currently implement `std::chars_format`.
// Annotate when it becomes available.
#if 0
    using std::chars_format;
    using std::to_chars;
    using std::from_chars;
#endif

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
