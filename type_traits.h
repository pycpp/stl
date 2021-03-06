//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Type detection utilities.
 */

#pragma once

#include <pycpp/stl/type_traits/disable_if.h>
#include <pycpp/stl/type_traits/endian.h>
#include <pycpp/stl/type_traits/has_reallocate.h>
#include <pycpp/stl/type_traits/is_aggregate.h>
#include <pycpp/stl/type_traits/is_array.h>
#include <pycpp/stl/type_traits/is_complete.h>
#include <pycpp/stl/type_traits/is_final.h>
#include <pycpp/stl/type_traits/is_invocable.h>
#include <pycpp/stl/type_traits/is_iterator.h>
#include <pycpp/stl/type_traits/is_relocatable.h>
#include <pycpp/stl/type_traits/is_safe_overload.h>
#include <pycpp/stl/type_traits/is_swappable.h>
#include <pycpp/stl/type_traits/is_trivial.h>
#include <pycpp/stl/type_traits/logical.h>
#include <pycpp/stl/type_traits/remove_cvref.h>
#include <pycpp/stl/type_traits/void_t.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Type deduction
using std::is_fundamental;
using std::is_arithmetic;
using std::is_scalar;
using std::is_object;
using std::is_compound;
using std::is_member_pointer;
using std::is_integral;
using std::is_floating_point;
using std::is_same;
using std::is_base_of;
using std::is_reference;
using std::is_void;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_class;
using std::is_enum;
using std::is_empty;
using std::is_union;
using std::is_trivial;
using std::is_standard_layout;
using std::is_literal_type;
using std::is_pod;
using std::is_polymorphic;
using std::is_abstract;
using std::is_signed;
using std::is_unsigned;
using std::is_function;
using std::is_pointer;
using std::is_member_object_pointer;
using std::is_member_function_pointer;
using std::common_type;

// Conversion
using std::underlying_type;

// Functions
using std::result_of;

// Check qualifiers
using std::is_const;
using std::is_volatile;

// Remove qualifiers
using std::remove_const;
using std::remove_volatile;
using std::remove_cv;
using std::remove_pointer;
using std::remove_reference;
using std::remove_extent;
using std::remove_all_extents;
using std::decay;

// Add qualifiers
using std::add_cv;
using std::add_const;
using std::add_volatile;
using std::add_lvalue_reference;
using std::add_rvalue_reference;
using std::add_pointer;

// Logic
using std::enable_if;
using std::conditional;

// Integral Constant
using std::integral_constant;

template <bool B>
using bool_constant = integral_constant<bool, B>;

template <size_t N>
using size_constant = integral_constant<size_t, N>;

using std::true_type;
using std::false_type;

// Convertible
using std::is_convertible;

// Constructible
using std::is_constructible;
using std::is_nothrow_constructible;
using std::is_default_constructible;
using std::is_nothrow_default_constructible;
using std::is_copy_constructible;
using std::is_nothrow_copy_constructible;
using std::is_move_constructible;
using std::is_nothrow_move_constructible;

// Assignable
using std::is_assignable;
using std::is_nothrow_assignable;
using std::is_copy_assignable;
using std::is_nothrow_copy_assignable;
using std::is_move_assignable;
using std::is_nothrow_move_assignable;

// Destructible
using std::is_destructible;
using std::is_trivially_destructible;
using std::is_nothrow_destructible;

// Memory
using std::aligned_storage;
using std::aligned_union;
using std::alignment_of;

// Property queries
using std::rank;
using std::extent;

// Sign Modifiers
using std::make_signed;
using std::make_unsigned;

// CONVENIENCE
// -----------

template <typename ... Ts>
using common_type_t = typename common_type<Ts...>::type;

template <typename Enum>
using underlying_type_t = typename underlying_type<Enum>::type;

template <typename T>
using result_of_t = typename result_of<T>::type;

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <typename T>
using decay_t = typename decay<T>::type;

template <typename T>
using add_cv_t = typename add_cv<T>::type;

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

template <
    size_t Len,
    size_t Align = alignof(typename aligned_storage<Len>::type)
>
using aligned_storage_t = typename aligned_storage<Len, Align>::type;

template <
    size_t Len,
    typename ... Ts
>
using aligned_union_t = typename aligned_union<Len, Ts...>::type;

template <typename T>
using make_signed_t = typename make_signed<T>::type;

template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

#if defined(PYCPP_CPP14)

template <typename T>
constexpr bool is_fundamental_v = is_fundamental<T>::value;

template <typename T>
constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <typename T>
constexpr bool is_scalar_v = is_scalar<T>::value;

template <typename T>
constexpr bool is_object_v = is_object<T>::value;

template <typename T>
constexpr bool is_compound_v = is_compound<T>::value;

template <typename T>
constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template <typename T>
constexpr bool is_integral_v = is_integral<T>::value;

template <typename T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

template <typename T, typename U>
constexpr bool is_base_of_v = is_base_of<T, U>::value;

template <typename T>
constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
constexpr bool is_void_v = is_void<T>::value;

template <typename T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <typename T>
constexpr bool is_class_v = is_class<T>::value;

template <typename T>
constexpr bool is_enum_v = is_enum<T>::value;

template <typename T>
constexpr bool is_empty_v = is_empty<T>::value;

template <typename T>
constexpr bool is_union_v = is_union<T>::value;

template <typename T>
constexpr bool is_trivial_v = is_trivial<T>::value;

template <typename T>
constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

template <typename T>
constexpr bool is_literal_type_v = is_literal_type<T>::value;

template <typename T>
constexpr bool is_pod_v = is_pod<T>::value;

template <typename T>
constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

template <typename T>
constexpr bool is_abstract_v = is_abstract<T>::value;

template <typename T>
constexpr bool is_signed_v = is_signed<T>::value;

template <typename T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <typename T>
constexpr bool is_function_v = is_function<T>::value;

template <typename T>
constexpr bool is_pointer_v = is_pointer<T>::value;

template <typename T>
constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

template <typename T>
constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

template <typename T>
constexpr bool is_const_v = is_const<T>::value;

template <typename T>
constexpr bool is_volatile_v = is_volatile<T>::value;

template <typename From, typename To>
constexpr bool is_convertible_v = is_convertible<From, To>::value;

template <typename T, typename ... Ts>
constexpr bool is_constructible_v = is_constructible<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Ts...>::value;

template <typename T>
constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

template <typename T>
constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

template <typename T>
constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template <typename T>
constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

template <typename T, typename ... Ts>
constexpr bool is_assignable_v = is_assignable<T, Ts...>::value;

template <typename T, typename ... Ts>
constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, Ts...>::value;

template <typename T>
constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

template <typename T>
constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

template <typename T>
constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

template <typename T>
constexpr bool is_destructible_v = is_destructible<T>::value;

template <typename T>
constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

template <typename T>
constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

template <typename T>
constexpr size_t alignment_of_v = alignment_of<T>::value;

template <typename T>
constexpr size_t rank_v = rank<T>::value;

template <typename T, unsigned N = 0>
constexpr size_t extent_v = extent<T, N>::value;

#endif

// MACROS
// ------

// Specify no-except conditions for 1-5 arguments
#define PYCPP_TYPE_ID(x) x
#define PYCPP_GET_TYPE(_1,_2,_3,_4,_5,NAME,...) NAME

// noexcept(noexcept(is_nothrow_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_default_constructible, T1>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_default_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_default_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_default_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_default_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_DEFAULT_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_copy_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_copy_constructible, T1>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_copy_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_copy_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_copy_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_copy_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_COPY_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_copy_assignable<Ts...>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_copy_assignable, T1>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_copy_assignable, T1, T2>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_copy_assignable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_copy_assignable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_copy_assignable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_COPY_ASSIGNABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_COPY_ASSIGNABLE5, PYCPP_NOEXCEPT_COPY_ASSIGNABLE4, PYCPP_NOEXCEPT_COPY_ASSIGNABLE3, PYCPP_NOEXCEPT_COPY_ASSIGNABLE2, PYCPP_NOEXCEPT_COPY_ASSIGNABLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_move_constructible<Ts...>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_move_constructible, T1>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_move_constructible, T1, T2>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_move_constructible, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_move_constructible, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_move_constructible, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE5, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE4, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE3, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE2, PYCPP_NOEXCEPT_MOVE_CONSTRUCTIBLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_move_assignable<Ts...>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_move_assignable, T1>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_move_assignable, T1, T2>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_move_assignable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_move_assignable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_move_assignable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_MOVE_ASSIGNABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE5, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE4, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE3, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE2, PYCPP_NOEXCEPT_MOVE_ASSIGNABLE1)(__VA_ARGS__))

// noexcept(noexcept(is_nothrow_swappable<Ts...>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE1(T1) noexcept(noexcept(template_map_and<is_nothrow_swappable, T1>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE2(T1, T2) noexcept(noexcept(template_map_and<is_nothrow_swappable, T1, T2>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE3(T1, T2, T3) noexcept(noexcept(template_map_and<is_nothrow_swappable, T1, T2, T3>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE4(T1, T2, T3, T4) noexcept(noexcept(template_map_and<is_nothrow_swappable, T1, T2, T3, T4>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE5(T1, T2, T3, T4, T5) noexcept(noexcept(template_map_and<is_nothrow_swappable, T1, T2, T3, T4, T5>::value))
#define PYCPP_NOEXCEPT_SWAPPABLE(...) PYCPP_TYPE_ID(PYCPP_GET_TYPE(__VA_ARGS__, PYCPP_NOEXCEPT_SWAPPABLE5, PYCPP_NOEXCEPT_SWAPPABLE4, PYCPP_NOEXCEPT_SWAPPABLE3, PYCPP_NOEXCEPT_SWAPPABLE2, PYCPP_NOEXCEPT_SWAPPABLE1)(__VA_ARGS__))

PYCPP_END_NAMESPACE
