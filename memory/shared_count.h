//  :copyright: (c) 2001, 2002, 2003 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `shared_ptr` reference counters.
 *
 *  Shared and weak counters for `shared_ptr` and `weak_ptr`,
 *  respectively. The thread-safe versions are implemented using
 *  atomic variables, which have generally better performance than
 *  spinlocks (and compile-down to a single `LOCK` instruction).
 *  The non-thread-safe versions use raw numeric types, for
 *  faster performance.
 *
 *  \synopsis
 *      class bad_weak_ptr: public std::exception
 *      {
 *      public:
 *          virtual char const* what() const throw();
 *      };
 *
 *      template <bool ThreadSafe>
 *      class sp_counted_base
 *      {
 *      public:
 *          sp_counted_base();
 *          sp_counted_base(const sp_counted_base&) = delete;
 *          sp_counted_base& operator=(const sp_counted_base&) = delete;
 *          virtual ~sp_counted_base();
 *
 *          virtual void dispose() = 0;
 *          virtual void destroy();
 *          virtual void* get_deleter(const std::type_info&) = 0;
 *          virtual void* get_untyped_deleter() = 0;
 *          void add_ref_copy();
 *          bool add_ref_lock();
 *          void release();
 *          void weak_add_ref();
 *          void weak_release();
 *          long use_count() const;
 *      };
 *
 *      template <bool ThreadSafe>
 *      class shared_count
 *      {
 *      public:
 *          using count_type = sp_counted_base<ThreadSafe>;
 *          static constexpr bool thread_safe = ThreadSafe;
 *
 *          constexpr shared_count() noexcept;
 *          shared_count(const shared_count& r);
 *          shared_count(shared_count&& r);
 *          explicit shared_count(const weak_count<thread_safe>& r);
 *          shared_count& operator=(const shared_count& r);
 *          ~shared_count();
 *
 *          template <typename T>
 *          explicit shared_count(T* p);
 *
 *          template <typename Pointer, typename Deleter>
 *          shared_count(Pointer p, Deleter d);
 *
 *          template <typename Pointer, typename Deleter, typename Allocator>
 *          shared_count(Pointer p, Deleter d, Allocator alloc);
 *
 *          template <typename U, typename Deleter>
 *          explicit shared_count(std::unique_ptr<U, Deleter>& r);
 *
 *          void swap(shared_count& r);
 *          bool unique() const;
 *          long use_count() const;
 *          bool empty() const;
 *
 *          friend bool operator==(const shared_count& x, const shared_count& y);
 *          friend bool operator<(const shared_count& x, const shared_count& y);
 *
 *          void* get_deleter(const std::type_info& ti) const;
 *          void* get_untyped_deleter() const;
 *      };
 *
 *      template <bool ThreadSafe>
 *      class weak_count
 *      {
 *      public:
 *          using count_type = sp_counted_base<ThreadSafe>;
 *          static constexpr bool thread_safe = ThreadSafe;
 *
 *          constexpr weak_count() noexcept;
 *          weak_count(const shared_count<thread_safe>& r) noexcept;
 *          weak_count(const weak_count& r) noexcept;
 *          weak_count(weak_count&& r) noexcept;
 *          ~weak_count();
 *          weak_count& operator=(const shared_count<thread_safe>& r);
 *          weak_count& operator=(const weak_count& r);
 *
 *          void swap(weak_count& r);
 *          long use_count() const;
 *          bool empty() const;
 *
 *          friend bool operator==(const weak_count& x, const weak_count& y);
 *          friend bool operator<(const weak_count& x, const weak_count& y);
 *      };
 */

#pragma once

#include <pycpp/stl/memory/allocator_traits.h>
#include <pycpp/stl/memory/checked_delete.h>
#include <pycpp/stl/thread/checked_thread.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <functional>
#include <typeinfo>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#ifndef PYCPP_SP_THREAD_SAFE
#   define PYCPP_SP_THREAD_SAFE true
#endif

// FORWARD
// -------

template <typename T, bool ThreadSafe = PYCPP_SP_THREAD_SAFE>
class shared_ptr;

template <typename T, bool ThreadSafe = PYCPP_SP_THREAD_SAFE>
class weak_ptr;

template <typename T, bool ThreadSafe = PYCPP_SP_THREAD_SAFE>
class enable_shared_from_this;

template <bool ThreadSafe = PYCPP_SP_THREAD_SAFE>
class shared_count;

template <bool ThreadSafe = PYCPP_SP_THREAD_SAFE>
class weak_count;

template <typename T>
struct is_relocatable;

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

// BAD WEAK PTR

class bad_weak_ptr: public std::exception
{
public:
    virtual
    char const*
    what()
    const throw()
    {
        return "bad_weak_ptr";
    }
};

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
    sp_counted_base& operator=(const sp_counted_base&) = delete;

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
        del_(ptr_);
    }

    virtual
    void
    destroy()
    {
        using allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<sp_counted_impl_pda>;
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
    using count_type = sp_counted_base<ThreadSafe>;
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

    template <typename Pointer, typename Deleter, typename Allocator>
    shared_count(
        Pointer p,
        Deleter d,
        Allocator alloc
    ):
        pi_(nullptr)
    {
        using impl_type = sp_counted_impl_pda<Pointer, Deleter, Allocator, thread_safe>;
        using impl_allocator = typename allocator_traits<Allocator>::template rebind_alloc<impl_type>;
        using impl_traits = allocator_traits<impl_allocator>;

        impl_allocator a(alloc);

        try {
            pi_ = a.allocate(1);
            impl_traits::construct(a, static_cast<impl_type*>(pi_), p, d, alloc);
        } catch(...) {
            d(p);
            if (pi_ != nullptr) {
                a.deallocate(static_cast<impl_type*>(pi_), 1);
            }
            throw;
        }
    }

    template <typename U, typename Deleter>
    explicit
    shared_count(
        std::unique_ptr<U, Deleter>& r
    ):
        pi_(nullptr)
    {
        using pointer = typename std::unique_ptr<U, Deleter>::pointer;
        using deleter_rr = typename std::conditional<
            std::is_reference<Deleter>::value,
            std::reference_wrapper<Deleter>,
            Deleter
        >::type;

        deleter_rr d(r.get_deleter());
        pi_ = new sp_counted_impl_pd<pointer, deleter_rr, thread_safe>(r.get(), d);

        // free unique_ptr
        r.release();
    }

    shared_count(
        const shared_count& r
    ):
        pi_(r.pi_)
    {
        if (pi_ != nullptr) {
            pi_->add_ref_copy();
        }
    }

    shared_count(
        shared_count&& r
    ):
        pi_(r.pi_)
    {
        r.pi_ = nullptr;
    }

    explicit
    shared_count(
        const weak_count<thread_safe>& r
    );

    // Assignment operators
    shared_count&
    operator=(
        const shared_count& r
    )
    {
        count_type* tmp = r.pi_;

        if (tmp != pi_) {
            if (tmp != nullptr) {
                tmp->add_ref_copy();
            }
            if (pi_ != nullptr) {
                pi_->release();
            }
            pi_ = tmp;
        }

        return *this;
    }

    // Destructors
    ~shared_count()
    {
        if (pi_ != nullptr) {
            pi_->release();
        }
    }

    // Modifiers
    void
    swap(
        shared_count& r
    )
    {
        count_type* tmp = r.pi_;
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
        return std::less<count_type*>()(x.pi_, y.pi_);
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
    template <bool> friend class shared_count;
    template <bool> friend class weak_count;

    count_type* pi_;
};

// WEAK COUNT

template <bool ThreadSafe>
class weak_count
{
public:
    using count_type = sp_counted_base<ThreadSafe>;
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
        count_type* tmp = r.pi_;

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
        count_type* tmp = r.pi_;

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
        count_type* tmp = r.pi_;
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
        return std::less<count_type*>()(x.pi_, y.pi_);
    }

private:
    template <bool> friend class shared_count;
    template <bool> friend class weak_count;

    count_type* pi_;
};

template <bool ThreadSafe>
shared_count<ThreadSafe>::shared_count(
    const weak_count<thread_safe>& r
):
    pi_(r.pi_)
{
    if (pi_ == nullptr || !pi_->add_ref_lock()) {
        throw bad_weak_ptr();
    }
}

// SPECIALIZATION
// --------------

template <bool ThreadSafe>
struct is_relocatable<shared_count<ThreadSafe>>: std::true_type
{};

template <bool ThreadSafe>
struct is_relocatable<weak_count<ThreadSafe>>: std::true_type
{};

PYCPP_END_NAMESPACE
