//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <bitset> aliases.
 */

#pragma once

#include <pycpp/stl/functional/hash_specialize.h>
#include <bitset>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::bitset;

// SPECIALIZATION
// --------------

PYCPP_SPECIALIZE_HASH_TEMPLATE(hash, size_t Size, bitset<Size>);

PYCPP_END_NAMESPACE
