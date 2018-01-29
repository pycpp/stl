//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
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

#if defined(PYCPP_CPP17)    // CPP17

using std::byte;

#else                       // <=CPP14

enum class byte: unsigned char
{};

#endif                      // CPP17

PYCPP_END_NAMESPACE
