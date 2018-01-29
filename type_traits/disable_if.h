//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Disable if condition is true.
 *
 *  Logical negation of `enable_if`, which disables substitution
 *  if the condition is true..
 *
 *  \synopsis
 *      template<bool B, typename T = void>
 *      using disable_if = implementation-defined;
 *
 *      template<bool B, typename T = void>
 *      using disable_if_t = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template<bool B, typename T = void>
using disable_if = std::enable_if<!B, T>;

template<bool B, typename T = void>
using disable_if_t = typename disable_if<B, T>::type;

PYCPP_END_NAMESPACE
