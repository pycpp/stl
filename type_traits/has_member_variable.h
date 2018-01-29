//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Check a type contains a specific member variable.
 *
 *  Macro to detect if a class contains a member variable of the given
 *  name (`member`), expanding to a struct inhering from `bool_constant`.
 *
 *  \synopsis
 *      #define PYCPP_HAS_MEMBER_VARIABLE(member, name)     implementation-defined
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Check if class has member variable.
 */
#define PYCPP_HAS_MEMBER_VARIABLE(member, name)                     \
    template <typename T, typename = int>                           \
    struct name: std::false_type                                    \
    {};                                                             \
                                                                    \
    template <typename T>                                           \
    struct name <T, decltype((void) T::member, 0)>: std::true_type  \
    {}


PYCPP_END_NAMESPACE
