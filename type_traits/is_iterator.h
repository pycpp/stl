//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Iterator utilities.
 *
 *  Iterator type deduction utilities. Supports two models, direct
 *  iterator tag detection (`is_*_iterator`), and iterator concept
 *  support (`is_*_iterable`). In addition, overloads to enable
 *  functions are exported, using the `enable_*` and `enable_*_t`
 *  models.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_input_iterator = implementation-defined;
 *
 *      template <typename T>
 *      using is_output_iterator = implementation-defined;
 *
 *      template <typename T>
 *      using is_forward_iterator = implementation-defined;
 *
 *      template <typename T>
 *      using is_bidirectional_iterator = implementation-defined;
 *
 *      template <typename T>
 *      using is_random_access_iterator = implementation-defined;
 *
 *      template <typename T>
 *      using is_input_iterable = implementation-defined;
 *
 *      template <typename T>
 *      using is_output_iterable = implementation-defined;
 *
 *      template <typename T>
 *      using is_forward_iterable = implementation-defined;
 *
 *      template <typename T>
 *      using is_bidirectional_iterable = implementation-defined;
 *
 *      template <typename T>
 *      using is_random_access_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_input_iterator = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_output_iterator = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_forward_iterator = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_bidirectional_iterator = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_random_access_iterator = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_input_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_output_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_forward_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_bidirectional_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_random_access_iterable = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_input_iterator_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_output_iterator_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_forward_iterator_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_bidirectional_iterator_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_random_access_iterator_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_input_iterable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_output_iterable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_forward_iterable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_bidirectional_iterable_t = implementation-defined;
 *
 *      template <typename T, typename R = void>
 *      using enable_random_access_iterable_t = implementation-defined;
 *
 *      #ifdef PYCPP_CPP14
 *
 *      template <typename T>
 *      constexpr bool is_input_iterator_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_output_iterator_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_forward_iterator_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_bidirectional_iterator_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_random_access_iterator_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_input_iterable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_output_iterable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_forward_iterable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_bidirectional_iterable_v = implementation-defined;
 *
 *      template <typename T>
 *      constexpr bool is_random_access_iterable_v = implementation-defined;
 *
 *      #endif
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <iterator>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// TYPE

template <typename T>
using is_input_iterator = std::is_same<
    std::input_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_output_iterator = std::is_same<
    std::output_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_forward_iterator = std::is_same<
    std::forward_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_bidirectional_iterator = std::is_same<
    std::bidirectional_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_random_access_iterator = std::is_same<
    std::random_access_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

// CONCEPTS

template <typename T>
using is_input_iterable = std::is_base_of<
    std::input_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_output_iterable = std::is_base_of<
    std::output_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_forward_iterable = std::is_base_of<
    std::forward_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_bidirectional_iterable = std::is_base_of<
    std::bidirectional_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T>
using is_random_access_iterable = std::is_base_of<
    std::random_access_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

// ENABLE IF

template <typename T, typename R = void>
using enable_input_iterator = std::enable_if<is_input_iterator<T>::value, R>;

template <typename T, typename R = void>
using enable_output_iterator = std::enable_if<is_output_iterator<T>::value, R>;

template <typename T, typename R = void>
using enable_forward_iterator = std::enable_if<is_forward_iterator<T>::value, R>;

template <typename T, typename R = void>
using enable_bidirectional_iterator = std::enable_if<is_bidirectional_iterator<T>::value, R>;

template <typename T, typename R = void>
using enable_random_access_iterator = std::enable_if<is_random_access_iterator<T>::value, R>;

template <typename T, typename R = void>
using enable_input_iterable = std::enable_if<is_input_iterable<T>::value, R>;

template <typename T, typename R = void>
using enable_output_iterable = std::enable_if<is_output_iterable<T>::value, R>;

template <typename T, typename R = void>
using enable_forward_iterable = std::enable_if<is_forward_iterable<T>::value, R>;

template <typename T, typename R = void>
using enable_bidirectional_iterable = std::enable_if<is_bidirectional_iterable<T>::value, R>;

template <typename T, typename R = void>
using enable_random_access_iterable = std::enable_if<is_random_access_iterable<T>::value, R>;

template <typename T, typename R = void>
using enable_input_iterator_t = typename enable_input_iterator<T, R>::type;

template <typename T, typename R = void>
using enable_output_iterator_t = typename enable_output_iterator<T, R>::type;

template <typename T, typename R = void>
using enable_forward_iterator_t = typename enable_forward_iterator<T, R>::type;

template <typename T, typename R = void>
using enable_bidirectional_iterator_t = typename enable_bidirectional_iterator<T, R>::type;

template <typename T, typename R = void>
using enable_random_access_iterator_t = typename enable_random_access_iterator<T, R>::type;

template <typename T, typename R = void>
using enable_input_iterable_t = typename enable_input_iterable<T, R>::type;

template <typename T, typename R = void>
using enable_output_iterable_t = typename enable_output_iterable<T, R>::type;

template <typename T, typename R = void>
using enable_forward_iterable_t = typename enable_forward_iterable<T, R>::type;

template <typename T, typename R = void>
using enable_bidirectional_iterable_t = typename enable_bidirectional_iterable<T, R>::type;

template <typename T, typename R = void>
using enable_random_access_iterable_t = typename enable_random_access_iterable<T, R>::type;

#ifdef PYCPP_CPP14

// SFINAE
// ------

// TYPE

template <typename T>
constexpr bool is_input_iterator_v = is_input_iterator<T>::value;

template <typename T>
constexpr bool is_output_iterator_v = is_output_iterator<T>::value;

template <typename T>
constexpr bool is_forward_iterator_v = is_forward_iterator<T>::value;

template <typename T>
constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;

template <typename T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;

// CONCEPTS

template <typename T>
constexpr bool is_input_iterable_v = is_input_iterable<T>::value;

template <typename T>
constexpr bool is_output_iterable_v = is_output_iterable<T>::value;

template <typename T>
constexpr bool is_forward_iterable_v = is_forward_iterable<T>::value;

template <typename T>
constexpr bool is_bidirectional_iterable_v = is_bidirectional_iterable<T>::value;

template <typename T>
constexpr bool is_random_access_iterable_v = is_random_access_iterable<T>::value;

#endif

PYCPP_END_NAMESPACE
