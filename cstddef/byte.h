//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `byte` backport for C++11.
 *
 *  \synopsis
 *      enum class byte: unsigned char
 *      {};
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)     // HAVE_CPP17

using std::byte;

#else                       // !HAVE_CPP17

enum class byte: unsigned char
{};

#endif                      // HAVE_CPP17

PYCPP_END_NAMESPACE
