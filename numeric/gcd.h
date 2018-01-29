//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `gcd` backport for C++11.
 *
 *  \synopsis
 *      template <typename T, typename U>
 *      inline constexpr
 *      typename std::common_type<T,U>::type
 *      gcd(T m, U n);
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/numeric/abs.h>
#include <numeric>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::gcd;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename T>
inline constexpr
T
gcd_impl(
    T m,
    T n
)
{
    static_assert((!std::is_signed<T>::value), "");
    return n == 0 ? m : gcd_impl<T>(n, m % n);
}


template <typename T, typename U>
inline constexpr
typename std::common_type<T,U>::type
gcd(
    T m,
    U n
)
{
    static_assert(
        std::is_integral<T>::value && std::is_integral<U>::value,
        "Arguments to gcd must be integer types"
    );
    static_assert(
        !std::is_same<typename std::remove_cv<T>::type, bool>::value,
        "First argument to gcd cannot be bool"
    );
    static_assert(
        !std::is_same<typename std::remove_cv<U>::type, bool>::value,
        "Second argument to gcd cannot be bool"
    );

    using integer_type = typename std::common_type<T, U>::type;
    using unsigned_type = typename std::make_unsigned<integer_type>::type;

    return static_cast<integer_type>(gcd_impl(
        static_cast<unsigned_type>(abs_constexpr<integer_type, T>()(m)),
        static_cast<unsigned_type>(abs_constexpr<integer_type, U>()(n)))
    );
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
