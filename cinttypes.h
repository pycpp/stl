//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cinttypes> aliases.
 */

#pragma once

#include <pycpp/stl/cstdint.h>
#include <cinttypes>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::imaxdiv_t;
using std::imaxabs;
using std::imaxdiv;
using std::strtoimax;
using std::strtoumax;
using std::wcstoimax;
using std::wcstoumax;
// GCC and Clang both do not support `std::abs` and `std::div`
// on extended-precision types. Don't export them.
//using std::abs;
//using std::div;

PYCPP_END_NAMESPACE
