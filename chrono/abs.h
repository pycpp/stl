//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `std::chrono::abs` backport for C++11.
 *
 *  \synopsis
 *      template <typename Rep, typename Period>
 *      inline constexpr
 *      chrono::duration<Rep, Period> abs(chrono::duration<Rep, Period> d);
 */

#pragma once

#include <pycpp/preprocessor/compiler_traits.h>
#include <pycpp/stl/type_traits/is_duration.h>

PYCPP_BEGIN_NAMESPACE

namespace chrono
{
// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::chrono::abs;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename Rep, typename Period>
inline PYCPP_CPP14_CONSTEXPR
typename std::enable_if<
    std::numeric_limits<Rep>::is_signed,
    std::chrono::duration<Rep, Period>
>::type
abs(
    std::chrono::duration<Rep, Period> d
)
{
    return d >= d.zero() ? d : -d;
}

#endif                              // CPP17

}   /* chrono */

PYCPP_END_NAMESPACE
