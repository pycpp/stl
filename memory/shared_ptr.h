//  :copyright: (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  :copyright: (c) 2004-2005 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `shared_ptr` with single-threaded optimizations.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/memory/checked_delete.h>
#include <pycpp/stl/thread/checked_thread.h>
#include <pycpp/stl/utility/fast_swap.h>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <typeinfo>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, bool ThreadSafe = true>
class shared_ptr;

template <typename T, bool ThreadSafe = true>
class weak_ptr;

template <typename T, bool ThreadSafe = true>
class enable_shared_from_this;

// TODO: restore
//class enable_shared_from_raw;

// FUNCTIONS
// ---------

inline
void
atomic_increment(
    std::atomic_int_least32_t* count
)
{
    count->fetch_add(1, std::memory_order_relaxed);
}


inline
std::int_least32_t
atomic_decrement(
    std::atomic_int_least32_t* count
)
{
    return count->fetch_sub( 1, std::memory_order_acq_rel );
}

inline
std::int_least32_t
atomic_conditional_increment(
    std::atomic_int_least32_t* count
)
{
    std::int_least32_t r = count->load(std::memory_order_relaxed);

    for (;;) {
        if (r == 0) {
            return r;
        }

        if(count->compare_exchange_weak(r, r + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
            return r;
        }
    }
}

// OBJECTS
// -------

// SP COUNTED BASE

template <bool ThreadSafe>
class sp_counted_base;

// Thread-safe
template <>
class sp_counted_base<true>
{
public:
    constexpr
    sp_counted_base():
        use_count_(1),
        weak_count_(1)
    {}

    sp_counted_base(const sp_counted_base&) = delete;
    sp_counted_base& operator=(const sp_counted_base&) = delete;

    virtual
    ~sp_counted_base()
    {}

    virtual void dispose() = 0;

    virtual
    void
    destroy()
    {
        delete this;
    }

    virtual void* get_deleter(const std::type_info&) = 0;
    virtual void* get_untyped_deleter() = 0;

    void
    add_ref_copy()
    {
        atomic_increment(&use_count_);
    }

    bool
    add_ref_lock()
    {
        return atomic_conditional_increment(&use_count_) != 0;
    }

    void
    release()
    {
        if (atomic_decrement(&use_count_) == 1) {
            dispose();
            weak_release();
        }
    }

    void
    weak_add_ref()
    {
        atomic_increment(&weak_count_);
    }

    void
    weak_release()
    {
        if (atomic_decrement( &weak_count_ ) == 1) {
            destroy();
        }
    }

    long
    use_count()
    const
    {
        return use_count_.load(std::memory_order_acquire);
    }

private:
    std::atomic_int_least32_t use_count_;
    std::atomic_int_least32_t weak_count_;
};


// Single-threaded
template <>
class sp_counted_base<false>: checked_thread<false>
{
public:
    sp_counted_base():
        use_count_(1),
        weak_count_(1)
    {
        this->check();
    }

    sp_counted_base(const sp_counted_base&) = delete;
    sp_counted_base & operator=(const sp_counted_base&) = delete;

    virtual
    ~sp_counted_base()
    {
        this->check();
    }

    virtual void dispose() = 0;

    virtual
    void
    destroy()
    {
        this->check();
        delete this;
    }

    virtual void* get_deleter(const std::type_info&) = 0;
    virtual void* get_untyped_deleter() = 0;

    void
    add_ref_copy()
    {
        this->check();
        ++use_count_;
    }

    bool
    add_ref_lock()
    {
        this->check();
        if (use_count_ == 0) {
            return false;
        }
        ++use_count_;
        return true;
    }

    void
    release()
    {
        this->check();
        if (--use_count_ == 0) {
            dispose();
            weak_release();
        }
    }

    void
    weak_add_ref()
    {
        this->check();
        ++weak_count_;
    }

    void
    weak_release()
    {
        this->check();
        if (--weak_count_ == 0) {
            destroy();
        }
    }

    long
    use_count()
    const
    {
        this->check();
        return use_count_;
    }

private:
    long use_count_;
    long weak_count_;
};

// SP COUNTED IMPL P

template <typename T, bool ThreadSafe>
class sp_counted_impl_p: public sp_counted_base<ThreadSafe>
{
public:
    explicit
    sp_counted_impl_p(
        T * p
    ):
        ptr_(p)
    {}

    sp_counted_impl_p(sp_counted_impl_p const &) = delete;
    sp_counted_impl_p& operator=(sp_counted_impl_p const &) = delete;

    virtual
    void
    dispose()
    {
        checked_delete(ptr_);
    }

    virtual
    void*
    get_deleter(
        const std::type_info&
    )
    {
        return nullptr;
    }

    virtual
    void*
    get_untyped_deleter()
    {
        return nullptr;
    }

private:
    T* ptr_;
};

// SP COUNTED IMPL PD

template <typename Pointer, typename Deleter, bool ThreadSafe>
class sp_counted_impl_pd: public sp_counted_base<ThreadSafe>
{
public:
    sp_counted_impl_pd(
        Pointer p,
        Deleter& d
    ):
        ptr_(p),
        del_(d)
    {}

    sp_counted_impl_pd(
        Pointer p
    ):
        ptr_(p),
        del_()
    {}

    sp_counted_impl_pd(sp_counted_impl_pd const &) = delete;
    sp_counted_impl_pd& operator=(sp_counted_impl_pd const &) = delete;

    virtual
    void
    dispose()
    {
        del_(ptr_);
    }

    virtual
    void*
    get_deleter(
        const  std::type_info& ti
    )
    noexcept
    {
        if (typeid(Deleter) == ti) {
            return &reinterpret_cast<char&>(del_);
        }
        return nullptr;
    }

    virtual
    void*
    get_untyped_deleter()
    noexcept
    {
        return &reinterpret_cast<char&>(del_);
    }

private:
    Pointer ptr_;
    Deleter del_;
};

// SP COUNTED IMPL PDA

template <typename Pointer, typename Deleter, typename Allocator, bool ThreadSafe>
class sp_counted_impl_pda: public sp_counted_base<ThreadSafe>
{
public:
    sp_counted_impl_pda(
        Pointer p,
        Deleter& d,
        Allocator alloc
    ):
        ptr_(p),
        del_(d),
        alloc_(alloc)
    {}

    sp_counted_impl_pda(
        Pointer p,
        Allocator alloc
    ):
        ptr_(p),
        del_(alloc),
        alloc_(alloc)
    {}

    sp_counted_impl_pda(const sp_counted_impl_pda&) = delete;
    sp_counted_impl_pda& operator=(const sp_counted_impl_pda&) = delete;

    virtual
    void
    dispose()
    {
        d_(ptr_);
    }

    virtual
    void
    destroy()
    {
        using allocator = typename allocator_traits<Allocator>::template rebind_alloc<sp_counted_impl_pda>;
        allocator alloc(alloc_);
        std::allocator_traits<allocator>::destroy(alloc, this);
        alloc.deallocate(this, 1);
    }

    virtual
    void*
    get_deleter(
        const std::type_info& ti
    )
    noexcept
    {
        if (typeid(Deleter) == ti) {
            return &reinterpret_cast<char&>(del_);
        }
        return nullptr;
    }

    virtual
    void*
    get_untyped_deleter()
    noexcept
    {
        return &reinterpret_cast<char&>(del_);
    }

private:
    Pointer ptr_;
    Deleter del_;
    Allocator alloc_;
};

// SHARED COUNT

template <bool ThreadSafe>
class shared_count
{
public:
    static constexpr bool thread_safe = ThreadSafe;

    constexpr
    shared_count()
    noexcept:
        pi_(nullptr)
    {}

    template <typename T>
    explicit
    shared_count(
        T* p
    ):
        pi_(nullptr)
    {
        try {
            pi_ = new sp_counted_impl_p<T, thread_safe>(p);
        } catch(...) {
            checked_delete(p);
            throw;
        }
    }

    template <typename Pointer, typename Deleter>
    shared_count(
        Pointer p,
        Deleter d
    ):
        pi_(nullptr)
    {
        try {
            pi_ = new sp_counted_impl_pd<Pointer, Deleter, thread_safe>(p, d);
        } catch(...) {
            d(p);
            throw;
        }
    }

    //
    // TODO: implement...

    ~shared_count()
    {
        if (pi_ != nullptr) {
            pi_->release();
        }
    }

    void
    swap(
        shared_count& r
    )
    {
        sp_counted_base<thread_safe>* tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long
    use_count()
    const
    {
        return pi_ != nullptr ? pi_->use_count() : 0;
    }

    bool
    unique()
    const
    {
        return use_count() == 1;
    }

    bool
    empty()
    const
    {
        return pi_ == nullptr;
    }

    friend inline
    bool
    operator==(
        const shared_count& x,
        const shared_count& y
    )
    {
        return x.pi_ == y.pi_;
    }

    friend inline
    bool
    operator<(
        const shared_count& x,
        const shared_count& y
    )
    {
        return std::less<sp_counted_base<thread_safe>*>()(x.pi_, y.pi_);
    }

    void*
    get_deleter(
        const std::type_info& ti
    )
    const
    {
        return pi_ ? pi_->get_deleter(ti) : nullptr;
    }

    void*
    get_untyped_deleter()
    const
    {
        return pi_ ? pi_->get_untyped_deleter() : nullptr;
    }

private:
    sp_counted_base<thread_safe>* pi_;
};

// WEAK COUNT

template <bool ThreadSafe>
class weak_count
{
public:
    static constexpr bool thread_safe = ThreadSafe;

    constexpr
    weak_count()
    noexcept:
        pi_(nullptr)
    {}

    weak_count(
        const shared_count<thread_safe>& r
    )
    noexcept:
        pi_(r.pi_)
    {
        if (pi_ != nullptr) {
            pi_->weak_add_ref();
        }
    }

    weak_count(
        const weak_count& r
    )
    noexcept:
        pi_(r.pi_)
    {
        if (pi_ != nullptr) {
            pi_->weak_add_ref();
        }
    }

    weak_count(
        weak_count&& r
    )
    noexcept:
        pi_(r.pi_)
    {
        r.pi_ = nullptr;
    }

    ~weak_count()
    {
        if (pi_ != nullptr) {
            pi_->weak_release();
        }
    }

    weak_count&
    operator=(
        const shared_count<thread_safe>& r
    )
    {
        sp_counted_base<thread_safe>* tmp = r.pi_;

        if(tmp != pi_ ) {
            if (tmp != nullptr) {
                tmp->weak_add_ref();
            }
            if (pi_ != nullptr) {
                pi_->weak_release();
            }
            pi_ = tmp;
        }

        return *this;
    }

    weak_count&
    operator=(
        const weak_count& r
    )
    {
        sp_counted_base<thread_safe>* tmp = r.pi_;

        if( tmp != pi_ ) {
            if (tmp != nullptr) {
                tmp->weak_add_ref();
            }
            if (pi_ != nullptr) {
                pi_->weak_release();
            }
            pi_ = tmp;
        }

        return *this;
    }

    void
    swap(
        weak_count& r
    )
    {
        sp_counted_base<thread_safe>* tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long
    use_count()
    const
    {
        return pi_ != nullptr ? pi_->use_count() : 0;
    }

    bool
    empty()
    const
    {
        return pi_ == nullptr;
    }

    friend inline
    bool
    operator==(
        const weak_count& x,
        const weak_count& y
    )
    {
        return x.pi_ == y.pi_;
    }

    friend inline
    bool
    operator<(
        const weak_count& x,
        const weak_count& y
    )
    {
        return std::less<sp_counted_base<thread_safe>*>()(x.pi_, y.pi_);
    }

private:
    template <bool>
    friend class shared_count;

    sp_counted_base<thread_safe>* pi_;
};

// HELPERS
// -------

// SP ENABLE SHARED FROM THIS

template <typename X, typename Y, typename T, bool ThreadSafe>
inline
void
sp_enable_shared_from_this(
    const shared_ptr<X, ThreadSafe>* spx,
    const Y* py,
    const enable_shared_from_this<T, ThreadSafe>* pe
)
{
    if (pe != nullptr) {
        pe->_internal_accept_owner(spx, const_cast<Y*>(py));
    }
}

// TODO: restore
//template <typename X, typename Y, bool ThreadSafe>
//inline
//void
//sp_enable_shared_from_this(
//    shared_ptr<X, ThreadSafe>* ppx,
//    const Y* py,
//    const enable_shared_from_raw* pe
//);


#ifdef _MANAGED

// Avoid C4793, ... causes native code generation

struct sp_any_pointer
{
    template <typename T>
    constexpr
    sp_any_pointer(
        T*
    )
    noexcept
    {}
};

inline
void
sp_enable_shared_from_this(
    sp_any_pointer,
    sp_any_pointer,
    sp_any_pointer
)
noexcept
{}

#else // _MANAGED

inline
void
sp_enable_shared_from_this(
    ...
)
noexcept
{}

#endif // _MANAGED

// SP POINTER CONSTRUCT

template <typename T, typename U, bool ThreadSafe>
inline
void
sp_pointer_construct(
    shared_ptr<T, ThreadSafe>* sp,
    U* p,
    shared_count<ThreadSafe>& count
)
noexcept
{
    shared_count<ThreadSafe>(p).swap(count);
    sp_enable_shared_from_this(sp, p, p);
}

template <typename T, typename U, bool ThreadSafe>
inline
void
sp_pointer_construct(
    shared_ptr<T[], ThreadSafe>*,
    U* p,
    shared_count<ThreadSafe>& count
)
noexcept
{
    static_assert(
        std::is_convertible<U[], T[]>::value,
        "U[] must be convertible to T[]."
    );

    using deleter = checked_array_deleter<T>;
    shared_count<ThreadSafe>(p, deleter()).swap(count);
}

template <typename T, size_t N, typename U, bool ThreadSafe>
inline void sp_pointer_construct(
    shared_ptr<T[N], ThreadSafe>*,
    U* p,
    shared_count<ThreadSafe>& count
)
noexcept
{
    static_assert(
        std::is_convertible<U[N], T[N]>::value,
        "U[N] must be convertible to T[N]."
    );

    using deleter = checked_array_deleter<T>;
    shared_count<ThreadSafe>(p, deleter()).swap(count);
}

// SP DELETER CONTRUCT

template <typename T, typename U, bool ThreadSafe>
inline
void
sp_deleter_construct(
    shared_ptr<T, ThreadSafe>* sp,
    U* p
)
noexcept
{
    sp_enable_shared_from_this(sp, p, p);
}


template <typename T, typename U, bool ThreadSafe>
inline
void
sp_deleter_construct(
    shared_ptr<T[], ThreadSafe>*,
    U*
)
{
    static_assert(
        std::is_convertible<U[], T[]>::value,
        "U[] must be convertible to T[]."
    );
}

template <typename T, size_t N, typename U, bool ThreadSafe>
inline
void
sp_deleter_construct(
    shared_ptr<T[N], ThreadSafe>*,
    U*
)
noexcept
{
    static_assert(
        std::is_convertible<U[N], T[N]>::value,
        "U[N] must be convertible to T[N]."
    );
}

// API
// ---

// ENABLE SHARED FROM THIS

// TODO: define

// SHARED PTR

template <typename T, bool ThreadSafe>
class shared_ptr
{
public:
    // MEMBER TYPES
    // ------------
    using element_type = typename std::remove_extent<T>::type;
    using weak_type = weak_ptr<T, ThreadSafe>;
    static constexpr bool thread_safe = ThreadSafe;

    // MEMBER FUNCTIONS
    // ----------------

    // Constructors
    constexpr
    shared_ptr()
    noexcept:
        ptr_(nullptr),
        ctrl_()
    {}

    constexpr
    shared_ptr(
        std::nullptr_t
    )
    noexcept:
        shared_ptr()
    {}

    template <typename U>
    explicit
    shared_ptr(
        U* p
    ):
        ptr_(p),
        ctrl_()
    {
        sp_pointer_construct(this, p, ctrl_);
    }

    template <typename U, typename Deleter>
    shared_ptr(
        U* p,
        Deleter d
    ):
        ptr_(p),
        ctrl_(p, d)
    {
        sp_deleter_construct(this, p);
    }

    template <typename Deleter>
    shared_ptr(
        std::nullptr_t p,
        Deleter d
    ):
        ptr_(p),
        ctrl_(p, d)
    {}

    template <typename U, typename Deleter, typename Allocator>
    shared_ptr(
        U* p,
        Deleter d,
        Allocator alloc
    ):
        ptr_(p),
        ctrl_(p, d, alloc)
    {
        sp_deleter_construct(this, p);
    }

    template <typename Deleter, typename Allocator>
    shared_ptr(
        std::nullptr_t p,
        Deleter d,
        Allocator alloc
    ):
        ptr_(p),
        ctrl_(p, d, alloc)
    {}

    template <typename U>
    shared_ptr(
        const shared_ptr<U, thread_safe>& r,
        element_type* p
    )
    noexcept:
        ptr_(p),
        ctrl_(r.ctrl_)
    {}

    shared_ptr(
        const shared_ptr& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_(r.ctrl_)
    {}

    template <typename U>
    shared_ptr(
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

    shared_ptr(
        shared_ptr&& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_()
    {
        ctrl_.swap(r.ctrl_);
        r.ptr_ = nullptr;
    }

    template <typename U>
    shared_ptr(
        shared_ptr<U, thread_safe>&& r
    )
    noexcept:
        ptr_(r.ptr_),
        ctrl_()
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        ctrl_.swap(r.ctrl_);
        r.ptr_ = nullptr;
    }

    template <typename U>
    explicit
    shared_ptr(
        const weak_ptr<U, thread_safe>& r
    ):
        ptr_(r.ptr_)
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        // it is now safe to copy r.ctrl_, as ptr_(r.ptr_) did not throw
        ctrl_ = r.ctrl_;
    }

    template <typename U, typename Deleter>
    shared_ptr(
        std::unique_ptr<U, Deleter>&& r
    ):
        ptr_(r.get()),
        ctrl_()
    {
        static_assert(
            std::is_convertible<U, T>::value,
            "U must be convertible to T."
        );

        using pointer = typename std::unique_ptr<U, Deleter>::pointer;
        pointer tmp = r.get();
        ctrl_ = shared_count<thread_safe>(r);

        sp_deleter_construct(this, tmp);
    }

    // Assignment
    shared_ptr&
    operator=(
        const shared_ptr& r
    )
    noexcept
    {
        shared_ptr(r).swap(*this);
        return *this;
    }

    template <typename U>
    shared_ptr&
    operator=(
        const shared_ptr<U, thread_safe>& r
    )
    noexcept
    {
        shared_ptr(r).swap(*this);
        return *this;
    }

    shared_ptr&
    operator=(
        shared_ptr&& r
    )
    noexcept
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }

    template <typename U>
    shared_ptr&
    operator=(
        shared_ptr<U, thread_safe>&& r
    )
    noexcept
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }

    template <typename U, typename Deleter>
    shared_ptr&
    operator=(
        std::unique_ptr<U, Deleter>&& r
    )
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }

    // Modifiers
    void
    reset()
    noexcept
    {
        shared_ptr().swap(*this);
    }

    template <typename U>
    void
    reset(
        U* p
    )
    {
        assert(p == nullptr || p != ptr_);
        shared_ptr(p).swap(*this);
    }

    template <typename U, typename Deleter>
    void
    reset(
        U* p,
        Deleter d
    )
    {
        shared_ptr(p, d).swap(*this);
    }

    template <typename U, typename Deleter, typename Allocator>
    void
    reset(
        U * p,
        Deleter d,
        Allocator alloc
    )
    {
        shared_ptr(p, d, alloc).swap(*this);
    }

    void
    swap(
        shared_ptr& r
    )
    noexcept
    {
        fast_swap(ptr_, r.ptr_);
        ctrl_.swap(r.ctrl_);
    }

    // Observers
    element_type*
    get()
    const noexcept
    {
        return ptr_;
    }

    T&
    operator*()
    const noexcept
    {
        assert(ptr_ != nullptr);
        return *ptr_;
    }

    T*
    operator->()
    const noexcept
    {
        assert(ptr_ != nullptr);
        return ptr_;
    }

    element_type&
    operator[](
        std::ptrdiff_t idx
    )
    {
        static constexpr size_t extent = std::extent<T>::value;
        assert(ptr_ != nullptr);
        assert(idx >= 0);
        assert(extent == 0 || idx < extent);

        return static_cast<element_type&>(ptr_[idx]);
    }

    long
    use_count()
    const noexcept
    {
        return ctrl_.use_count();
    }

    bool
    unique()
    const noexcept
    {
        return ctrl_.unique();
    }

    explicit
    operator bool()
    const noexcept
    {
        return ptr_ != nullptr;
    }

    template <typename U>
    bool
    owner_before(
        const shared_ptr<U, thread_safe>& r
    )
    const noexcept
    {
        return ctrl_ < r.ctrl_;
    }

    template <typename U>
    bool
    owner_before(
        const weak_ptr<U, thread_safe>& r
    )
    const noexcept
    {
        return ctrl_ < r.ctrl_;
    }

private:
    element_type* ptr_;
    shared_count<thread_safe> ctrl_;
};

// NON MEMBER FUNCTIONS


// TODO:

// http://www.boost.org/doc/libs/1_61_0/boost/smart_ptr/shared_ptr.hpp


// TODO: implement thread_safe;

PYCPP_END_NAMESPACE
