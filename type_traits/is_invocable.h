//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `is_invocable*` backports for C++11.
 *
 *  \warning The `is_nothrow_*` variants cannot  be effectively backported
 *  prior to C++17, due to the omission of `noexcept` from the type
 *  system.
 *
 *  \synopsis
 *      template <typename F, typename ... Ts>
 *      struct invoke_result: implementation-defined
 *      {};
 *
 *      template <typename F, typename ... Ts>
 *      struct is_invocable: implementation-defined
 *      {};
 *
 *      template <typename R, typename F, typename ... Ts>
 *      struct is_invocable_r: implementation-defined
 *      {};
 *
 *      template <typename F, typename ... Ts>
 *      using invoke_result_t = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename F, typename ... Ts>
 *      constexpr bool is_invocable_v = implementation-defined;
 *
 *      template <class R, typename F, typename ... Ts>
 *      constexpr bool is_invocable_r_v = implementation-defined;
 *
 *      #endif
 *
 *      #ifdef PYCPP_CPP17
 *
 *      template <typename F, typename ... Ts>
 *      struct is_nothrow_invocable: implementation-defined
 *      {};
 *
 *      template <typename R, typename F, typename ... Ts>
 *      struct is_nothrow_invocable_r: implementation-defined
 *      {};
 *
 *      template <typename F, typename ... Ts>
 *      constexpr bool is_nothrow_invocable_v = implementation-defined;
 *
 *      template <class R, typename F, typename ... Ts>
 *      constexpr bool is_nothrow_invocable_r_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/stl/type_traits/any_overload.h>
#include <pycpp/stl/type_traits/is_reference_wrapper.h>
#include <pycpp/stl/type_traits/nat.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP17)            // CPP17

using std::is_invocable;
using std::is_invocable_r;
using std::is_nothrow_invocable;
using std::is_nothrow_invocable_r;
using std::invoke_result;
using std::invoke_result_t;

#else                               // <=CPP14

// MACROS
// ------

#define PYCPP_INVOKE_RETURN(...) noexcept(noexcept(__VA_ARGS__))        \
     -> decltype(__VA_ARGS__)                                           \
{                                                                       \
    return __VA_ARGS__;                                                 \
}

// FORWARD
// ------

template <typename ... Ts>
auto invoke_impl(any_overload, Ts&&... ts) -> nat;

template <typename ... Ts>
auto invoke_constexpr_impl(any_overload, Ts&&... ts) -> nat;

// HELPERS
// -------

template <typename DecayedF>
struct member_pointer_class_type
{};

template <typename R, typename ClassT>
struct member_pointer_class_type<R ClassT::*>
{
    using type = ClassT;
};

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type,
    typename ClassT = typename member_pointer_class_type<DecayedF>::type
>
using enable_if_bullet1 = typename std::enable_if<
    std::is_member_function_pointer<DecayedF>::value
    && std::is_base_of<ClassT, DecayedA>::value
>::type;

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type
>
using enable_if_bullet2 = typename std::enable_if<
    std::is_member_function_pointer<DecayedF>::value &&
    is_reference_wrapper<DecayedA>::value
>::type;

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type,
    typename ClassT = typename member_pointer_class_type<DecayedF>::type
>
using enable_if_bullet3 = typename std::enable_if<
    std::is_member_function_pointer<DecayedF>::value &&
    !std::is_base_of<ClassT, DecayedA>::value &&
    !is_reference_wrapper<DecayedA>::value
>::type;

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type,
    typename ClassT = typename member_pointer_class_type<DecayedF>::type
>
using enable_if_bullet4 = typename std::enable_if<
    std::is_member_object_pointer<DecayedF>::value &&
    std::is_base_of<ClassT, DecayedA>::value
>::type;

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type
>
using enable_if_bullet5 = typename std::enable_if<
    std::is_member_object_pointer<DecayedF>::value &&
    is_reference_wrapper<DecayedA>::value
>::type;

template <
    typename F,
    typename A,
    typename DecayedF = typename std::decay<F>::type,
    typename DecayedA = typename std::decay<A>::type,
    typename ClassT = typename member_pointer_class_type<DecayedF>::type
>
using enable_if_bullet6 = typename std::enable_if<
    std::is_member_object_pointer<DecayedF>::value &&
    !std::is_base_of<ClassT, DecayedA>::value &&
    !is_reference_wrapper<DecayedA>::value
>::type;

// FUNCTIONS
// ---------

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet1<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN((std::forward<A>(a).*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet1<F, A>
>
inline constexpr
auto
invoke_constexpr_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN((std::forward<A>(a).*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet2<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN((a.get().*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet2<F, A>
>
inline constexpr
auto
invoke_constexpr_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN((a.get().*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet3<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN(((*std::forward<A>(a)).*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename ... Ts,
     typename = enable_if_bullet3<F, A>
>
inline
constexpr auto
invoke_constexpr_impl(F&& f, A&& a, Ts&&... ts)
PYCPP_INVOKE_RETURN(((*std::forward<A>(a)).*f)(std::forward<Ts>(ts)...));

template <
     typename F,
     typename A,
     typename = enable_if_bullet4<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN(std::forward<A>(a).*f);

template <
     typename F,
     typename A,
     typename = enable_if_bullet4<F, A>
>
inline constexpr
auto
invoke_constexpr_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN(std::forward<A>(a).*f);

template <
     typename F,
     typename A,
     typename = enable_if_bullet5<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN(a.get().*f);

template <
     typename F,
     typename A,
     typename = enable_if_bullet5<F, A>
>
inline constexpr
auto
invoke_constexpr_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN(a.get().*f);

template <
     typename F,
     typename A,
     typename = enable_if_bullet6<F, A>
>
inline
auto
invoke_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN((*std::forward<A>(a)).*f);

template <
     typename F,
     typename A,
     typename = enable_if_bullet6<F, A>
>
inline constexpr
auto
invoke_constexpr_impl(F&& f, A&& a)
PYCPP_INVOKE_RETURN((*std::forward<A>(a)).*f);

template <typename F, typename ... Ts>
inline
auto
invoke_impl(F&& f, Ts&&... ts)
PYCPP_INVOKE_RETURN(std::forward<F>(f)(std::forward<Ts>(ts)...));

template <typename F, typename ... Ts>
inline constexpr
auto
invoke_constexpr_impl(F&& f, Ts&&... ts)
PYCPP_INVOKE_RETURN(std::forward<F>(f)(std::forward<Ts>(ts)...));

// OBJECTS
// -------

template <typename R, typename F, typename ... Ts>
struct invokable_r_impl
{
    using result_type = decltype(invoke_impl(std::declval<F>(), std::declval<Ts>()...));

    using type =
        typename std::conditional<
            !std::is_same<result_type, nat>::value,
            typename std::conditional<
                std::is_void<R>::value,
                std::true_type,
                std::is_convertible<result_type, R>
            >::type,
            std::false_type
        >::type;

    static constexpr bool value = type::value;
};

template <typename F, typename ... Ts>
using invokable_impl = invokable_r_impl<void, F, Ts...>;

template <typename F, typename ... Ts>
struct invoke_of_impl: public std::enable_if<
        invokable_impl<F, Ts...>::value,
        typename invokable_r_impl<void, F, Ts...>::result_type
    >
{};

// API
// ---

template <typename F, typename ... Ts>
struct invoke_result: invoke_of_impl<F, Ts...>
{};

template <typename F, typename ... Ts>
struct is_invocable:
    std::integral_constant<bool, invokable_impl<F, Ts...>::value>
{};

template <typename R, typename F, typename ... Ts>
struct is_invocable_r:
    std::integral_constant<bool, invokable_r_impl<R, F, Ts...>::value>
{};

template <typename F, typename ... Ts>
using invoke_result_t = typename invoke_result<F, Ts...>::type;

// CLEANUP
// -------

#undef PYCPP_INVOKE_RETURN

#endif                              // CPP17

#if defined(PYCPP_CPP14)            // CPP14

// SFINAE
// ------

template <typename F, typename ... Ts>
constexpr bool is_invocable_v = is_invocable<F, Ts...>::value;

template <class R, typename F, typename ... Ts>
constexpr bool is_invocable_r_v = is_invocable_r<R, F, Ts...>::value;

#endif                              // CPP14

#if defined(PYCPP_CPP17)

// SFINAE
// ------

template <typename F, typename ... Ts>
constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<F, Ts...>::value;

template <class R, typename F, typename ... Ts>
constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, F, Ts...>::value;

#endif                              // CPP17

PYCPP_END_NAMESPACE
