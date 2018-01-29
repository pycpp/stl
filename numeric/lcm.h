//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `lcm` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, typename U>
 *      inline constexpr
 *      typename std::common_type<T,U>::type
 *      lcm(T m, U n);
 */

#pragma once

#include <pycpp/stl/numeric/gcd.h>
#include <cassert>
#include <limits>

PYCPP_BEGIN_NAMESPACE

#if defined(PYCPP_CPP17)            // CPP17

using std::lcm;

#else                               // <=CPP14

// FUNCTIONS
// ---------

// Get constexpr abs.
template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
lcm_impl4(
    T m,
    U n
)
{
    using integer_type = typename std::common_type<T, U>::type;
    return abs_constexpr<integer_type, U>()(n);
}


// Divide by GCD.
template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
lcm_impl3(
    T m,
    U n
)
{
    using integer_type = typename std::common_type<T, U>::type;
    return abs_constexpr<integer_type, T>()(m) / gcd(m, n);
}


// Multiple after dividing by GCD.
template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
lcm_impl2(
    T m,
    U n
)
{
    return lcm_impl3(m, n) * lcm_impl4(m, n);
}


// Check for 0
template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
lcm_impl1(
    T m,
    U n
)
{
    return (m == 0 || n == 0) ? 0 : lcm_impl2(m, n);
}


template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
lcm(
    T m,
    U n
)
{
    static_assert(
        std::is_integral<T>::value && std::is_integral<U>::value,
        "Arguments to lcm must be integer types"
    );
    static_assert(
        !std::is_same<typename std::remove_cv<T>::type, bool>::value,
        "First argument to lcm cannot be bool"
    );
    static_assert(
        !std::is_same<typename std::remove_cv<U>::type, bool>::value,
        "Second argument to lcm cannot be bool"
    );

    return lcm_impl1(m, n);
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
