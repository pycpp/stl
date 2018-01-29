//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `not_fn` backport for C++11.
 *
 *  \synopsis
 *      template <typename F>
 *      implementation-defined not_fn(F&& f);
 */

#pragma once

#include <pycpp/stl/functional/invoke.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::not_fn;

#else                               // <=CPP14

// FUNCTIONS
// ---------

template <typename DecayedFunc>
class not_fn_impl
{
    DecayedFunc f_;

public:
    not_fn_impl() = delete;

    template <typename ... Ts>
    auto
    operator()(
        Ts&&... ts
    ) &
    noexcept(noexcept(!invoke(f_, std::forward<Ts>(ts)...)))
    -> decltype(!invoke(f_, std::forward<Ts>(ts)...))
    {
        return !invoke(f_, std::forward<Ts>(ts)...);
    }

    template <typename ... Ts>
    auto
    operator()(
        Ts&&... ts
    ) &&
    noexcept(noexcept(!invoke(std::move(f_), std::forward<Ts>(ts)...)))
    -> decltype(!invoke(std::move(f_), std::forward<Ts>(ts)...))
    {
        return !invoke(std::move(f_), std::forward<Ts>(ts)...);
    }

    template <typename ... Ts>
    auto
    operator()(
        Ts&&... ts
    ) const&
    noexcept(noexcept(!invoke(f_, std::forward<Ts>(ts)...)))
    -> decltype(!invoke(f_, std::forward<Ts>(ts)...))
    {
        return !invoke(f_, std::forward<Ts>(ts)...);
    }

    template <typename ... Ts>
    auto
    operator()(
        Ts&&... ts
    ) const&&
    noexcept(noexcept(!invoke(std::move(f_), std::forward<Ts>(ts)...)))
    -> decltype(!invoke(std::move(f_), std::forward<Ts>(ts)...))
    {
        return !invoke(std::move(f_), std::forward<Ts>(ts)...);
    }

private:
    template <
        typename RawFunc,
        typename = typename std::enable_if<
            !std::is_same<typename std::decay<RawFunc>::type, not_fn_impl>::value
        >::type
    >
    explicit
    not_fn_impl(
        RawFunc&& rf
    ):
        f_(std::forward<RawFunc>(rf))
    {}

    template <typename RawFunc>
    friend inline
    not_fn_impl<typename std::decay<RawFunc>::type>
    not_fn(RawFunc&&);
};


template <typename RawFunc>
inline
not_fn_impl<typename std::decay<RawFunc>::type>
not_fn(RawFunc&& f)
{
    return not_fn_impl<typename std::decay<RawFunc>::type>(std::forward<RawFunc>(f));
}

#endif                              // CPP17

PYCPP_END_NAMESPACE
