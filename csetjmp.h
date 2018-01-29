//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <csetjmp> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <csetjmp>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::jmp_buf;
using std::longjmp;

PYCPP_END_NAMESPACE
