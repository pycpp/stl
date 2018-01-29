//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `endian` backport for C++11.
 *
 *  \synopsis
 *      enum class endian
 *      {
 *          little = implementation-defined,
 *          big = implementation-defined,
 *          native = implementation-defined
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/preprocessor/compiler.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP20)            // CPP20

using std::endian;

#else                               // <=CPP17

enum class endian
{
    little = LITTLE_ENDIAN,
    big = BIG_ENDIAN,
    native = BYTE_ORDER,
};

#endif                              // CPP20

PYCPP_END_NAMESPACE
