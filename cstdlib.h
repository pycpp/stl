//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cstdlib> aliases.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/cstdlib/aligned_alloc.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

//Types
using std::size_t;
using std::div_t;
using std::ldiv_t;
using std::lldiv_t;

// Process control
using std::abort;
using std::exit;
using std::_Exit;
using std::atexit;
using std::system;
using std::getenv;

// MinGW doesn't necessarily support these.
// Flow control is via the macros _GLIBCXX_HAVE_QUICK_EXIT
// Defining them when the features aren't present won't work.
#if !defined(PYCPP_MINGW) || defined(_GLIBCXX_HAVE_QUICK_EXIT)
    using std::quick_exit;
#endif

#if !defined(PYCPP_MINGW) || defined(_GLIBCXX_HAVE_AT_QUICK_EXIT)
    using std::at_quick_exit;
#endif

// Memory management
using std::malloc;
using std::calloc;
using std::realloc;
using std::free;

// Numeric string conversion
using std::atof;
using std::atoi;
using std::atol;
using std::atoll;
using std::strtol;
using std::strtoll;
using std::strtoul;
using std::strtoull;
using std::strtof;
using std::strtod;
using std::strtold;

// Wide string manipulation
using std::mblen;
using std::mbtowc;
using std::wctomb;
using std::mbstowcs;
using std::wcstombs;

// Miscellaneous algorithms and math
using std::rand;
using std::srand;
using std::qsort;
using std::bsearch;
using std::abs;
using std::labs;
using std::llabs;
using std::div;
using std::ldiv;
using std::lldiv;

PYCPP_END_NAMESPACE
