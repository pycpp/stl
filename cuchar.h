//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cuchar> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cuchar>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Functions
using std::mbrtoc16;
using std::c16rtomb;
using std::mbrtoc32;
using std::c32rtomb;

PYCPP_END_NAMESPACE
