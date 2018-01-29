//  :copyright: (c) 2001, 2002, 2003 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief `weak_ptr` with single-threaded optimizations.
 *
 *  \synopsis
 *      template <typename T, bool ThreadSafe>
 *      class weak_ptr
 *      {
 *      public:
 *          using element_type = typename std::remove_extent<T>::type;
 *          using count_type = weak_count<ThreadSafe>;
 *          static constexpr bool thread_safe = ThreadSafe;
 *
 *          constexpr weak_ptr() noexcept;
 *          weak_ptr(const weak_ptr& r);
 *          weak_ptr(weak_ptr&& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr(const weak_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr(const shared_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr(weak_ptr<U, thread_safe>&& r) noexcept;
 *
 *          weak_ptr& operator=(const weak_ptr& r) noexcept;
 *          weak_ptr& operator=(weak_ptr&& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr& operator=(const weak_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr& operator=(const shared_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          weak_ptr& operator=(weak_ptr<U, thread_safe>&& r) noexcept;
 *
 *          void swap(weak_ptr& x) noexcept;
 *          void reset() noexcept;
 *
 *          long use_count() const noexcept;
 *          bool expired() const noexcept;
 *          shared_ptr<T, thread_safe> lock() const noexcept;
 *
 *          template <typename U>
 *          bool owner_before(const weak_ptr<U, thread_safe>& x) const noexcept;
 *
 *          template <typename U>
 *          bool owner_before(const shared_ptr<U, thread_safe>& x) const noexcept;
 *      };
 *
 *      template <typename T, bool ThreadSafe>
 *      void swap(weak_ptr<T, ThreadSafe>& x, weak_ptr<T, ThreadSafe>& y);
 */

#pragma once

#include <pycpp/stl/memory/shared_count.h>
#include <pycpp/stl/utility/fast_swap.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename T, bool ThreadSafe>
class weak_ptr
{
public:
    // MEMBER TYPES
    // ------------
    using element_type = typename std::remove_extent<T>::type;
    using count_type = weak_count<ThreadSafe>;
    static constexpr bool thread_safe = ThreadSafe;

    // Constructors
    constexpr
    weak_ptr()
    noexcept:
        ptr_(nullptr),
        ctrl_()
    {}

    weak_ptr(
        const weak_ptr& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_(r.ctrl_)
    {}

    template <typename U>
    weak_ptr(
        const weak_ptr<U, thread_safe>& r
    )
    noexcept:
        ptr_(r.lock().get()),
        ctrl_(r.ctrl_)
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );
    }

    template <typename U>
    weak_ptr(
        const shared_ptr<U, thread_safe>& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_(r.ctrl_)
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );
    }

    weak_ptr(
        weak_ptr&& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_(std::move(r.ctrl_))
    {
        r.ptr_ = nullptr;
    }

    template <typename U>
    weak_ptr(
        weak_ptr<U, thread_safe>&& r
    )
    noexcept:
        ptr_(r.lock().get()),
        ctrl_(std::move(r.ctrl_))
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        r.ptr_ = nullptr;
    }

    // Assignment
    weak_ptr& operator=(
        const weak_ptr& r
    )
    noexcept
    {
        ptr_ = r.ptr_;
        ctrl_ = r.ctrl_;
        return *this;
    }

    template <typename U>
    weak_ptr&
    operator=(
        const weak_ptr<U, thread_safe>& r
    )
    noexcept
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        ptr_ = r.lock().get();
        ctrl_ = r.ctrl_;
        return *this;
    }

    template <typename U>
    weak_ptr&
    operator=(
        const shared_ptr<U, thread_safe>& r
    )
    noexcept
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        ptr_ = r.ptr_;
        ctrl_ = r.ctrl_;
        return *this;
    }

    weak_ptr& operator=(
        weak_ptr&& r
    )
    noexcept
    {
        swap(r);
        return *this;
    }

    template <typename U>
    weak_ptr&
    operator=(
        weak_ptr<U, thread_safe>&& r
    )
    noexcept
    {
        swap(r);
        return *this;
    }

    // Modifiers
    void
    swap(
        weak_ptr& x
    )
    noexcept
    {
        fast_swap(ptr_, x.ptr_);
        ctrl_.swap(x.ctrl_);
    }

    void
    reset()
    noexcept
    {
        weak_ptr().swap(*this);
    }

    // Observers
    long
    use_count()
    const noexcept
    {
        return ctrl_.use_count();
    }

    bool
    expired()
    const noexcept
    {
        return ctrl_.use_count() == 0;
    }

    shared_ptr<T, thread_safe>
    lock()
    const noexcept
    {
        return shared_ptr<T, thread_safe>(*this);
    }

    template <typename U>
    bool
    owner_before(const weak_ptr<U, thread_safe>& x)
    const noexcept
    {
        return ptr_ < x.ptr_;
    }

    template <typename U>
    bool
    owner_before(const shared_ptr<U, thread_safe>& x)
    const noexcept
    {
        return ptr_ < x.ptr_;
    }

private:
    element_type* ptr_;
    count_type ctrl_;

    template <typename, bool> friend class weak_ptr;
    template <typename, bool> friend class shared_ptr;
};


template <typename T, bool ThreadSafe>
void swap(
    weak_ptr<T, ThreadSafe>& x,
    weak_ptr<T, ThreadSafe>& y
)
noexcept
{
    x.swap(y);
}

// SPECIALIZATION
// --------------

template <typename T, bool ThreadSafe>
struct is_relocatable<weak_ptr<T, ThreadSafe>>: is_relocatable<weak_count<ThreadSafe>>
{};

PYCPP_END_NAMESPACE
