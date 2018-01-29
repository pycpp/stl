//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Not A Type definition.
 *
 *  \synopsis
 *      struct nat
 *      {
 *          nat() = delete;
 *          nat(const nat&) = delete;
 *          nat& operator=(const nat&) = delete;
 *          ~nat() = delete;
 *      };
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

struct nat
{
    nat() = delete;
    nat(const nat&) = delete;
    nat& operator=(const nat&) = delete;
    ~nat() = delete;
};

PYCPP_END_NAMESPACE
