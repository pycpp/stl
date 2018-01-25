//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type definition which accepts arguments passed to it.
 *
 *  \synopsis
 *      struct any_overload
 *      {
 *          any_overload(...);
 *      };
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

struct any_overload
{
    any_overload(...);
};

PYCPP_END_NAMESPACE
