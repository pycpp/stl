//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `std::chrono::round` backport for C++11.
 */

#pragma once

#include <pycpp/stl/chrono/floor.h>

PYCPP_BEGIN_NAMESPACE

namespace chrono
{
// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::chrono::round;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename ToDuration, typename Rep, typename Period>
inline CPP14_CONSTEXPR
typename std::enable_if<
    is_duration<ToDuration>::value,
    ToDuration
>::type
round(
    const std::chrono::duration<Rep, Period>& d
)
{
    ToDuration lower = floor<ToDuration>(d);
    ToDuration upper = lower + ToDuration{1};
    auto lower_diff = d - lower;
    auto upper_diff = upper - d;
    if (lower_diff < upper_diff) {
        return lower;
    }
    if (lower_diff > upper_diff) {
        return upper;
    }
    return lower.count() & 1 ? upper : lower;
}

template <typename ToDuration, typename Clock, typename Duration>
inline CPP14_CONSTEXPR
typename std::enable_if<
    is_duration<ToDuration>::value,
    std::chrono::time_point<Clock, ToDuration>
>::type
round(
    const std::chrono::time_point<Clock, Duration>& t
)
{
    return std::chrono::time_point<Clock, ToDuration>{round<ToDuration>(t.time_since_epoch())};
}

#endif                              // CPP17

}   /* chrono */

PYCPP_END_NAMESPACE
