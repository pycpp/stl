//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cfenv> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cfenv>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::fenv_t;
using std::fexcept_t;

// Functions
using std::feclearexcept;
using std::fetestexcept;
using std::feraiseexcept;
using std::fegetexceptflag;
using std::fesetexceptflag;
using std::fegetround;
using std::fesetround;
using std::fegetenv;
using std::fesetenv;
using std::feholdexcept;
using std::feupdateenv;

PYCPP_END_NAMESPACE
