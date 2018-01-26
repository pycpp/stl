//  :copyright: (c) 2002-2003 Peter Dimov.
//  :copyright: (c) 2003 Daniel Frey.
//  :copyright: (c) 2003 Howard Hinnant.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check deletion of complete types.
 *
 *  \synopsis
 *      template <typename T>
 *      void checked_delete(T* p);
 *
 *      template <typename T>
 *      void checked_array_delete(T* p);
 *
 *      template <typename T>
 *      struct checked_deleter
 *      {
 *          using result_type = implementation-defined;
 *          using argument_type = implementation-defined;
 *
 *          void operator()(T* p) const;
 *      };
 *
 *      template <typename T>
 *      struct checked_array_deleter
 *      {
 *          using result_type = implementation-defined;
 *          using argument_type = implementation-defined;
 *
 *          void operator()(T* p) const;
 *      };
 */

#pragma once

#include <pycpp/stl/type_traits/is_complete.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

template <typename T>
inline
void
checked_delete(
    T* p
)
{
    static_assert(is_complete<T>::value, "Type must be complete.");
    delete p;
}

template <typename T>
inline
void
checked_array_delete(
    T* p
)
{
    static_assert(is_complete<T>::value, "Type must be complete.");
    delete[] p;
}

// OBJECTS
// -------

template <typename T>
struct checked_deleter
{
    using result_type = void;
    using argument_type = T*;

    void
    operator()(
        T* p
    )
    const
    {
        checked_delete(p);
    }
};

template <typename T>
struct checked_array_deleter
{
    using result_type = void;
    using argument_type = T*;

    void
    operator()(
        T* p
    )
    const
    {
        checked_array_delete(p);
    }
};

PYCPP_END_NAMESPACE
