//  :copyright: (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  :copyright: (c) 2004-2005 Peter Dimov.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `shared_ptr` with single-threaded optimizations.
 *
 *  \synopsis
 *      template <typename T, bool ThreadSafe>
 *      class enable_shared_from_this
 *      {
 *      protected:
 *          constexpr enable_shared_from_this() noexcept;
 *          constexpr enable_shared_from_this(const enable_shared_from_this&);
 *          enable_shared_from_this& operator=(const enable_shared_from_this&) noexcept;
 *          ~enable_shared_from_this() noexcept;
 *
 *      public:
 *          static constexpr bool thread_safe = ThreadSafe;
 *
 *          shared_ptr<T, thread_safe> shared_from_this();
 *          shared_ptr<const T, thread_safe> shared_from_this() const;
 *          weak_ptr<T, thread_safe> weak_from_this() noexcept;
 *          weak_ptr<const T, thread_safe> weak_from_this() const noexcept;
 *      };
 *
 *      template <typename T, bool ThreadSafe>
 *      class shared_ptr
 *      {
 *      public:
 *          using element_type = typename std::remove_extent<T>::type;
 *          using weak_type = weak_ptr<T, ThreadSafe>;
 *          using count_type = shared_count<ThreadSafe>;
 *          static constexpr bool thread_safe = ThreadSafe;
 *
 *          constexpr shared_ptr() noexcept;
 *          constexpr shared_ptr(std::nullptr_t) noexcept;
 *          shared_ptr(const shared_ptr& r) noexcept;
 *          shared_ptr(shared_ptr&& r) noexcept;
 *
 *          template <typename U>
 *          explicit shared_ptr(U* p);
 *
 *          template <typename U, typename Deleter>
 *          shared_ptr( U* p, Deleter d);
 *
 *          template <typename Deleter>
 *          shared_ptr(std::nullptr_t p, Deleter d);
 *
 *          template <typename U, typename Deleter, typename Allocator>
 *          shared_ptr(U* p, Deleter d, Allocator alloc);
 *
 *          template <typename Deleter, typename Allocator>
 *          shared_ptr(std::nullptr_t p, Deleter d, Allocator alloc);
 *
 *          template <typename U>
 *          shared_ptr(const shared_ptr<U, thread_safe>& r, element_type* p) noexcept;
 *
 *          template <typename U>
 *          shared_ptr(const shared_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          shared_ptr(shared_ptr<U, thread_safe>&& r) noexcept;
 *
 *          template <typename U>
 *          explicit shared_ptr(const weak_ptr<U, thread_safe>& r);
 *
 *          template <typename U, typename Deleter>
 *          shared_ptr(std::unique_ptr<U, Deleter>&& r);
 *
 *          shared_ptr& operator=(const shared_ptr& r) noexcept;
 *          shared_ptr& operator=(shared_ptr&& r) noexcept;
 *
 *          template <typename U>
 *          shared_ptr& operator=(const shared_ptr<U, thread_safe>& r) noexcept;
 *
 *          template <typename U>
 *          shared_ptr& operator=(shared_ptr<U, thread_safe>&& r) noexcept;
 *
 *          template <typename U, typename Deleter>
 *          shared_ptr& operator=(std::unique_ptr<U, Deleter>&& r);
 *
 *          void reset() noexcept;
 *          void swap(shared_ptr& r) noexcept;
 *
 *          template <typename U>
 *          void reset(U* p);
 *
 *          template <typename U, typename Deleter>
 *          void reset(U* p, Deleter d);
 *
 *          template <typename U, typename Deleter, typename Allocator>
 *          void reset(U * p, Deleter d, Allocator alloc);
 *
 *          element_type* get() const noexcept;
 *          T& operator*() const noexcept;
 *          T* operator->() const noexcept;
 *          element_type& operator[](std::ptrdiff_t idx);
 *          long use_count() const noexcept;
 *          bool unique() const noexcept;
 *          explicit operator bool() const noexcept;
 *
 *          template <typename U>
 *          bool owner_before(const shared_ptr<U, thread_safe>& r) const noexcept;
 *
 *          template <typename U>
 *          bool owner_before(const weak_ptr<U, thread_safe>& r) const noexcept;
 *      };
 *
 *      template <typename T, bool ThreadSafe, typename ... Ts>
 *      shared_ptr<T, ThreadSafe> make_shared(Ts&&... ts);
 *
 *      template <typename T, typename ... Ts>
 *      shared_ptr<T, true> make_shared(Ts&&... ts);
 *
 *      template <typename T, bool ThreadSafe, typename Allocator, typename ... Ts>
 *      shared_ptr<T, ThreadSafe> allocate_shared(const Allocator& alloc, Ts&&... ts);
 *
 *      template <typename T, typename Allocator, typename ... Ts>
 *      shared_ptr<T, true> allocate_shared(const Allocator& alloc, Ts&&... ts);
 */

#pragma once

#include <pycpp/stl/memory/weak_ptr.h>
#include <pycpp/stl/memory/make_shared.h>
#include <pycpp/stl/type_traits/is_array.h>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

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
);

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

template <typename T, bool ThreadSafe>
class enable_shared_from_this
{
protected:
    // Constructors
    constexpr
    enable_shared_from_this()
    noexcept
    {}

    constexpr
    enable_shared_from_this(
        const enable_shared_from_this&
    )
    noexcept
    {}

    enable_shared_from_this&
    operator=(
        const enable_shared_from_this&
    )
    noexcept
    {
        return *this;
    }

    // Destructors
    ~enable_shared_from_this()
    noexcept
    {}

public:
    static constexpr bool thread_safe = ThreadSafe;

    shared_ptr<T, thread_safe>
    shared_from_this()
    {
        shared_ptr<T, thread_safe> p(weak_);
        assert(p.get() == this);
        return p;
    }

    shared_ptr<const T, thread_safe>
    shared_from_this()
    const
    {
        shared_ptr<T const, thread_safe> p(weak_);
        assert(p.get() == this);
        return p;
    }

    weak_ptr<T, thread_safe>
    weak_from_this()
    noexcept
    {
        return weak_;
    }

    weak_ptr<const T, thread_safe>
    weak_from_this()
    const noexcept
    {
        return weak_;
    }

private:
    mutable weak_ptr<T, thread_safe> weak_;

    template <typename X, typename Y, typename U, bool Threaded>
    friend
    void
    sp_enable_shared_from_this(
        const shared_ptr<X, Threaded>* spx,
        const Y* py,
        const enable_shared_from_this<U, Threaded>* pe
    );

    template <typename X, typename Y>
    void accept_owner(
        const shared_ptr<X, thread_safe>* ppx,
        Y* py
    )
    const noexcept
    {
        if(weak_.expired() ) {
            weak_ = shared_ptr<T, thread_safe>(*ppx, py);
        }
    }
};

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
        pe->accept_owner(spx, const_cast<Y*>(py));
    }
}

// SHARED PTR

template <typename T, typename R>
using sp_disable_if_array_t = typename std::enable_if<
    !is_array<T>::value, R
>::type;

template <typename T, typename R>
using sp_enable_if_array_t = typename std::enable_if<
    is_array<T>::value, R
>::type;

template <typename T, typename R>
using sp_enable_if_bounded_array_t = typename std::enable_if<
    is_bounded_array<T>::value, R
>::type;

template <typename T, typename R>
using sp_enable_if_unbounded_array_t = typename std::enable_if<
    is_unbounded_array<T>::value, R
>::type;

template <typename T, bool ThreadSafe, typename ... Ts>
sp_disable_if_array_t<T, shared_ptr<T, ThreadSafe>>
make_shared(Ts&&... ts);

// TODO(ahuszagh): Wish list
//template <typename T, bool ThreadSafe>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared();
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(const typename std::remove_extent<T>::type& u);
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(std::size_t n);
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(std::size_t n, const typename std::remove_extent<T>::type& u);

template <typename T, bool ThreadSafe, typename Allocator, typename ... Ts>
sp_disable_if_array_t<T, shared_ptr<T, ThreadSafe>>
allocate_shared(const Allocator& alloc, Ts&&... ts);

// TODO(ahuszagh): Wish list
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc);
//
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, const typename std::remove_extent<T>::type& u);
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, std::size_t n);
//
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, std::size_t n, const typename std::remove_extent<T>::type& u);


template <typename T, bool ThreadSafe>
class shared_ptr
{
public:
    // MEMBER TYPES
    // ------------
    using element_type = typename std::remove_extent<T>::type;
    using weak_type = weak_ptr<T, ThreadSafe>;
    using count_type = shared_count<ThreadSafe>;
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
        ctrl_ = count_type(r.ctrl_);
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
        ctrl_ = count_type(r);

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
    count_type ctrl_;

    // Friends
    template <typename, bool> friend class weak_ptr;
    template <typename, bool> friend class shared_ptr;

    template <typename U, bool Threaded, typename ... Ts>
    friend sp_disable_if_array_t<U, shared_ptr<U, Threaded>> make_shared(Ts&&...);

    template <typename U, bool Threaded, typename Allocator, typename ... Ts>
    friend sp_disable_if_array_t<U, shared_ptr<U, Threaded>> allocate_shared(const Allocator&, Ts&&...);

    // Internal
    void*
    get_deleter(
        const std::type_info& ti
    )
    const noexcept
    {
        return ctrl_.get_deleter(ti);
    }

    void*
    get_untyped_deleter()
    const noexcept
    {
        return ctrl_.get_untyped_deleter();
    }
};

// MAKE SHARED

template <typename T, bool ThreadSafe, typename ... Ts>
inline
sp_disable_if_array_t<T, shared_ptr<T, ThreadSafe>>
make_shared(
    Ts&&... ts
)
{
    static_assert(std::is_constructible<T, Ts...>::value, "Can't construct object in make_shared");

    using allocator_type = allocator<T>;
    using shared_type = shared_ptr<T, ThreadSafe>;
    using deleter_type = sp_ms_deleter<T>;

    // allocate control and data
    shared_type ptr(static_cast<T*>(nullptr), deleter_type());
    auto* pd = static_cast<deleter_type*>(ptr.get_untyped_deleter());
    void* pv = pd->address();
    new (pv) T(std::forward<Ts>(ts)...);

    // initialize and create shared
    pd->set_initialized();
    T* ptr2 = static_cast<T*>(pv);
    sp_enable_shared_from_this(&ptr, ptr2, ptr2);

    return shared_type(ptr, ptr2);
}

template <typename T, typename ... Ts>
inline
sp_disable_if_array_t<T, shared_ptr<T, PYCPP_SP_THREAD_SAFE>>
make_shared(
    Ts&&... ts
)
{
    return make_shared<T, PYCPP_SP_THREAD_SAFE>(std::forward<Ts>(ts)...);
}

// TODO: need all make_shared

//template <typename T, bool ThreadSafe>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared();
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(const typename std::remove_extent<T>::type& u);
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(std::size_t n);
//
//template <typename T, bool ThreadSafe>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//make_shared(std::size_t n, const typename std::remove_extent<T>::type& u);

// ALLOCATE SHARED

template <typename T, bool ThreadSafe, typename Allocator, typename ... Ts>
inline
sp_disable_if_array_t<T, shared_ptr<T, ThreadSafe>>
allocate_shared(
    const Allocator& alloc,
    Ts&&... ts
)
{
    static_assert(std::is_constructible<T, Ts...>::value, "Can't construct object in make_shared");

    using element_type = T;
    using allocator_type = Allocator;
    using alloc_traits = allocator_traits<allocator_type>;
    using shared_type = shared_ptr<element_type, ThreadSafe>;
    using deleter_type = sp_as_deleter<element_type, allocator_type>;

    // allocate control and data
    allocator_type a(alloc);
    shared_type ptr(static_cast<T*>(nullptr), deleter_type(a), a);
    auto* pd = static_cast<deleter_type*>(ptr.get_untyped_deleter());
    void* pv = pd->address();
    alloc_traits::construct(a, static_cast<T*>(pv), std::forward<Ts>(ts)...);

    // initialize and create shared
    pd->set_initialized();
    T* ptr2 = static_cast<T*>(pv);
    sp_enable_shared_from_this(&ptr, ptr2, ptr2);

    return shared_type(ptr, ptr2);
}

template <typename T, typename Allocator, typename ... Ts>
inline
sp_disable_if_array_t<T, shared_ptr<T, PYCPP_SP_THREAD_SAFE>>
allocate_shared(
    const Allocator& alloc,
    Ts&&... ts
)
{
    return allocate_shared<T, PYCPP_SP_THREAD_SAFE>(alloc, std::forward<Ts>(ts)...);
}

// TODO: need all allocate_shared

//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc)
//
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, const typename std::remove_extent<T>::type& u);
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, std::size_t n);
//
//template <typename T, bool ThreadSafe, typename Allocator>
//sp_enable_if_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(const Allocator& alloc, std::size_t n, const typename std::remove_extent<T>::type& u);

// Tentative implementation of #1
//template <typename T, bool ThreadSafe, typename Allocator>
//inline
//sp_enable_if_bounded_array_t<T, shared_ptr<T, ThreadSafe>>
//allocate_shared(
//    const Allocator& alloc
//)
//{
//    constexpr size_t size = std::extent<T, 0>::value;
//    using axis_type = typename std::remove_extent<T>::type;
//    using value_type = typename std::remove_all_extents<T>::type;
//    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<value_type> type;
//    // TODO: do I need state?
//    // sp_array_scalar is remove_all_extents
//}

// TODO: implement swap

// SPECIALIZATION
// --------------

template <typename T, bool ThreadSafe>
struct is_relocatable<enable_shared_from_this<T, ThreadSafe>>: is_relocatable<weak_ptr<T, ThreadSafe>>
{};

template <typename T, bool ThreadSafe>
struct is_relocatable<shared_ptr<T, ThreadSafe>>: is_relocatable<shared_count<ThreadSafe>>
{};

PYCPP_END_NAMESPACE
