//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <cctype> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cctype>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Functions
using std::isalnum;
using std::isalpha;
using std::islower;
using std::isupper;
using std::isdigit;
using std::isxdigit;
using std::iscntrl;
using std::isgraph;
using std::isspace;
using std::isblank;
using std::isprint;
using std::ispunct;
using std::tolower;
using std::toupper;

PYCPP_END_NAMESPACE
