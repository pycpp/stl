//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `std::chrono::ceil` backport for C++11.
 *
 *  \synopsis
 *      template <typename ToDuration, typename Rep, typename Period>
 *      inline constexpr
 *      ToDuration ceil(const chrono::duration<Rep, Period>& d);
 *
 *      template <typename ToDuration, typename Clock, typename Duration>
 *      inline constexpr
 *      chrono::time_point<Clock, ToDuration> ceil(const chrono::time_point<Clock, Duration>& t);
 */

#pragma once

#include <pycpp/preprocessor/compiler_traits.h>
#include <pycpp/stl/type_traits/is_duration.h>

PYCPP_BEGIN_NAMESPACE

namespace chrono
{
// ALIAS
// -----

#if defined(HAVE_CPP17)             // CPP17

using std::chrono::ceil;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename ToDuration, typename Rep, typename Period>
inline CPP14_CONSTEXPR
typename std::enable_if<
    is_duration<ToDuration>::value,
    ToDuration
>::type
ceil(
    const std::chrono::duration<Rep, Period>& d
)
{
    ToDuration t = std::chrono::duration_cast<ToDuration>(d);
    if (t < d) {
        t = t + ToDuration{1};
    }
    return t;
}

template <typename ToDuration, typename Clock, typename Duration>
inline CPP14_CONSTEXPR
typename std::enable_if<
    is_duration<ToDuration>::value,
    std::chrono::time_point<Clock, ToDuration>
>::type
ceil(
    const std::chrono::time_point<Clock, Duration>& t
)
{
    return std::chrono::time_point<Clock, ToDuration>{ceil<ToDuration>(t.time_since_epoch())};
}

#endif                              // CPP17

}   /* chrono */

PYCPP_END_NAMESPACE
