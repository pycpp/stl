//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <clocale> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <clocale>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::lconv;

// Functions
using std::setlocale;
using std::localeconv;

PYCPP_END_NAMESPACE
