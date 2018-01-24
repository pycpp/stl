//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <typeindex> aliases.
 */

#pragma once

#include <pycpp/stl/functional/hash_specialize.h>
#include <typeindex>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::type_index;

// SPECIALIZATION
// --------------

PYCPP_SPECIALIZE_HASH_VALUE(hash, type_index);

PYCPP_END_NAMESPACE
