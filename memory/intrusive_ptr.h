//  :copyright: (c) 2001-2002 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Intrusive pointer with embedded reference counting.
 *
 *  \synopsis
// TODO:
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/container/compressed_pair.h>
#include <cassert>
#include <cstddef>
#include <functional>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// INTRUSIVE PTR TRAITS

// Dummy traits for the intrusive pointer.
// Contains two functions taking the raw pointer,
// `add_ref`, which increases the reference count,
// and release, which releases the pointer.
// This class will be compressed, so unless reference
// counting is required in the class, it will
// not increase the class size.
template <typename T>
struct intrusive_ptr_traits
{
    using element_type = T;

    static
    void
    add_ref(
        element_type* p
    )
    {}

    static
    void
    release(
        element_type* p
    )
    {}
};

// INTRUSIVE PTR

template <typename T, typename TraitsType = intrusive_ptr_traits<T>>
class intrusive_ptr
{
public:
    using element_type = T;
    using traits_type = TraitsType;

    // Constructors
    constexpr
    intrusive_ptr()
    noexcept:
        ptr_(nullptr)
    {}

    intrusive_ptr(
        element_type* p,
        bool add_ref = true
    ):
        ptr_(p)
    {
        if (ptr_ != nullptr && add_ref) {
            traits_type::add_ref(ptr_);
        }
    }

    template <typename U>
    intrusive_ptr(
        const intrusive_ptr<U, traits_type>& x
    ):
        ptr_(x.ptr_)
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );
        if (ptr_ != nullptr) {
            traits_type::add_ref(ptr_);
        }
    }

    intrusive_ptr(
        const intrusive_ptr& x
    ):
        ptr_(x.ptr_)
    {
        if (ptr_ != nullptr) {
            traits_type::add_ref(ptr_);
        }
    }

    template <typename U>
    intrusive_ptr(
        intrusive_ptr<U, traits_type>&& x
    )
    noexcept:
        ptr_(x.ptr_)
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );
        x.ptr_ = nullptr;
    }

    intrusive_ptr(
        intrusive_ptr&& x
    ):
        ptr_(x.ptr_)
    {
        x.ptr_ = nullptr;
    }

    ~intrusive_ptr()
    {
        if (ptr_ != nullptr) {
            traits_type::release(ptr_);
        }
    }

    // Assignment
    template <typename U>
    intrusive_ptr&
    operator=(
        const intrusive_ptr<U, traits_type>& x
    )
    noexcept
    {
        intrusive_ptr(x).swap(*this);
        return *this;
    }

    intrusive_ptr&
    operator=(
        const intrusive_ptr& x
    )
    noexcept
    {
        intrusive_ptr(x).swap(*this);
        return *this;
    }

    template <typename U>
    intrusive_ptr&
    operator=(
        intrusive_ptr<U, traits_type>&& x
    )
    noexcept
    {
        intrusive_ptr(move(x)).swap(*this);
        return *this;
    }

    intrusive_ptr&
    operator=(
        intrusive_ptr&& x
    )
    noexcept
    {
        intrusive_ptr(move(x)).swap(*this);
        return *this;
    }

    intrusive_ptr&
    operator=(element_type* x)
    {
        intrusive_ptr(x).swap(*this);
        return *this;
    }

    // Observers
    element_type*
    get()
    const noexcept
    {
        return ptr_;
    }

    element_type&
    operator*()
    const noexcept
    {
        assert(ptr_ != nullptr);
        return *ptr_;
    }

    element_type*
    operator->()
    const noexcept
    {
        assert(ptr_ != nullptr);
        return ptr_;
    }

    explicit
    operator bool()
    const noexcept
    {
        return ptr_ != nullptr;
    }

    // Modifiers
    void
    reset()
    {
        intrusive_ptr().swap(*this);
    }

    void
    reset(
        element_type* x
    )
    {
        intrusive_ptr(x).swap(*this);
    }

    void
    reset(
        element_type* x,
        bool add_ref
    )
    {
        intrusive_ptr(x, add_ref).swap(*this);
    }

    element_type*
    release()
    noexcept
    {
        element_type* p = ptr_;
        ptr_ = 0;
        return p;
    }

    void
    swap(
        intrusive_ptr& x
    )
    noexcept
    {
        std::swap(ptr_, x.ptr_);
    }

private:
    element_type* ptr_;

    template <typename, typename> friend class intrusive_ptr;
};

// Relative operators -- intrusive_ptr vs. intrusive_ptr

template <typename T, typename TraitsType>
bool
operator==(
    intrusive_ptr<T, TraitsType>& x,
    intrusive_ptr<T, TraitsType>& y
)
{
    return x.get() == y.get();
}

template <typename T, typename TraitsType>
inline
bool
operator!=(
    const intrusive_ptr<T, TraitsType>& x,
    const intrusive_ptr<T, TraitsType>& y
)
{
    return !(x == y);
}

template <typename T, typename TraitsType>
inline
bool
operator<(
    const intrusive_ptr<T, TraitsType>& x,
    const intrusive_ptr<T, TraitsType>& y
)
{
    return x.get() < y.get();
}

template <typename T, typename TraitsType>
inline
bool
operator>(
    const intrusive_ptr<T, TraitsType>& x,
    const intrusive_ptr<T, TraitsType>& y
)
{
    return y < x;
}

template <typename T, typename TraitsType>
inline
bool
operator>=(
    const intrusive_ptr<T, TraitsType>& x,
    const intrusive_ptr<T, TraitsType>& y
)
{
    return !(x < y);
}

template <typename T, typename TraitsType>
inline
bool
operator<=(
    const intrusive_ptr<T, TraitsType>& x,
    const intrusive_ptr<T, TraitsType>& y
)
{
    return !(y < x);
}

// Swap

template <typename T, typename TraitsType>
void
swap(
    intrusive_ptr<T, TraitsType>& x,
    intrusive_ptr<T, TraitsType>& y
)
{
    x.swap(y);
}

// SPECIALIZATION
// --------------

template <typename T>
struct hash;

template <typename T, typename TraitsType>
struct hash<intrusive_ptr<T, TraitsType>>
{
    using argument_type = intrusive_ptr<T, TraitsType>;

    inline
    size_t
    operator()(
        const argument_type& x
    )
    const noexcept
    {
        using element_type = typename argument_type::element_type;
        return hash<element_type*>()(x.get());
    }
};

PYCPP_END_NAMESPACE
