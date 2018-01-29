//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Compile-time absolute value.
 *
 *  \synopsis
 *      template <typename Result, typename Source>
 *      struct abs_constexpr<Result, Sourc>
 *      {
 *          constexpr Result operator()(Source t) const noexcept;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <limits>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

template <typename Result, typename Source, bool IsSigned = std::is_signed<Source>::value>
struct abs_constexpr;

template <typename Result, typename Source>
struct abs_constexpr<Result, Source, true>
{
    constexpr
    Result
    negative(
        Source t
    )
    const noexcept
    {
        return t == std::numeric_limits<Source>::min() ? -static_cast<Result>(t) : -t;
    }

    constexpr
    Result
    operator()(
        Source t
    )
    const noexcept
    {
        return t >= 0 ? t : negative(t);
    }
};

template <typename Result, typename Source>
struct abs_constexpr<Result, Source, false> {
    constexpr
    Result
    operator()(
        Source t
    )
    const noexcept
    {
        return t;
    }
};

PYCPP_END_NAMESPACE
