//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <climits> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <climits>

// MACROS
// ------

#ifndef __CHAR_BIT__
#   define __CHAR_BIT__ 8
#endif

#ifndef CHAR_BIT
#   define CHAR_BIT __CHAR_BIT__
#endif
