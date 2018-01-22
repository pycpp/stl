//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `identity` type wrapper.
 *
 *  \synopsis
 *      template <typename T>
 *      using identity = implementation-defined;
 *
 *      template <typename T>
 *      using identity_t = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename T>
struct identity
{
    using type = T;
};

template <typename T>
using identity_t = typename identity<T>::type;

PYCPP_END_NAMESPACE
