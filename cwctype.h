//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cwctype> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cwctype>

PYCPP_BEGIN_NAMESPACE

// ALIASES
// -------

// Functions
using std::iswalnum;
using std::iswalpha;
using std::iswlower;
using std::iswupper;
using std::iswdigit;
using std::iswxdigit;
using std::iswcntrl;
using std::iswgraph;
using std::iswspace;
using std::iswblank;
using std::iswprint;
using std::iswpunct;
using std::iswctype;
using std::wctype;
using std::towlower;
using std::towupper;
using std::towctrans;
using std::wctrans;

PYCPP_END_NAMESPACE
