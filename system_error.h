//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <system_error>.
 */

#pragma once

#include <pycpp/stl/functional/hash_specialize.h>
#include <system_error>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Classes
using std::error_category;
using std::error_condition;
using std::errc;
using std::error_code;
using std::system_error;
using std::is_error_code_enum;
using std::is_error_condition_enum;

// Functions
using std::generic_category;
using std::system_category;
using std::make_error_code;
using std::make_error_condition;

// SPECIALIZATION
// --------------

PYCPP_SPECIALIZE_HASH_VALUE(hash, error_code);

template <>
struct hash<error_condition>
{
    using argument_type = error_condition;
    using result_type = size_t;

    size_t
    operator()(
        argument_type x
    )
    const noexcept
    {
        return std::hash<int>()(x.value());
    }
};


PYCPP_END_NAMESPACE
