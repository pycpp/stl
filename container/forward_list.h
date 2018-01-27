//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL forward_list with allocator erasure from iterators.
 *
 *  \synopsis
 *       template <
 *          typename T,
 *          typename VoidPtr = void*
 *      >
 *      class forward_list_facet
 *      {
 *      public:
 *          using value_type = T;
 *          using reference = value_type&;
 *          using const_reference = const value_type&;
 *          using pointer = implementation-defined;
 *          using const_pointer = implementation-defined;
 *          using size_type = Size;
 *          using difference_type = DiffT;
 *          using iterator = implementation-defined;
 *          using const_iterator = implementation-defined;
 *
 *          // Element access
 *          reference front();
 *          const_reference front() const;
 *
 *          // Iterators
 *          iterator before_begin() noexcept;
 *          const_iterator before_begin() const noexcept;
 *          const_iterator cbefore_begin() const noexcept;
 *          iterator begin() noexcept;
 *          const_iterator begin() const noexcept;
 *          const_iterator cbegin() const noexcept;
 *          iterator end() noexcept;
 *          const_iterator end() const noexcept;
 *          const_iterator cend() const noexcept;
 *
 *          // Capacity
 *          bool empty() const noexcept;
 *          size_type max_size() const noexcept;
 *          // TODO: continue
 *      };
 *
 *      template <
 *          typename T,
 *          typename Allocator = allocator<T>
 *      >
 *      class forward_list
 *      {
 *      public:
 *          using value_type = T;
 *          using allocator_type = Allocator;
 *          using reference = value_type&;
 *          using const_reference = const value_type&;
 *          using size_type = implementation-defined;
 *          using difference_type = implementation-defined;
 *          using pointer = implementation-defined;
 *          using const_pointer = implementation-defined;
 *          using iterator = implementation-defined;
 *          using const_iterator = implementation-defined;
 *
 *          // Constructors
 *
 *          // Element access
 *          reference front();
 *          const_reference front() const;
 *
 *          // Iterators
 *          iterator before_begin() noexcept;
 *          const_iterator before_begin() const noexcept;
 *          const_iterator cbefore_begin() const noexcept;
 *          iterator begin() noexcept;
 *          const_iterator begin() const noexcept;
 *          const_iterator cbegin() const noexcept;
 *          iterator end() noexcept;
 *          const_iterator end() const noexcept;
 *          const_iterator cend() const noexcept;
 *
 *          // Capacity
 *          bool empty() const noexcept;
 *          size_type max_size() const noexcept;
 *          // TODO: continue
 *      };
 */

#pragma once

#include <pycpp/stl/container/compressed_pair.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
// TODO: need functional
// TODO: remove std::

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, typename VoidPtr>
struct forward_list_node;

template <typename NodePtr>
struct forward_begin_node;

// OBJECTS
// -------

// FORWARD LIST NODE VALUE TYPE

template <typename>
struct forward_list_node_value_type;

template <typename T, typename VoidPtr>
struct forward_list_node_value_type<forward_list_node<T, VoidPtr>>
{
    using type = T;
};

// FORWARD NODE TRAITS

template <typename NodePtr>
struct forward_node_traits
{
    using node = remove_cv_t<
        typename pointer_traits<NodePtr>::element_type
    >;
    using node_value_type = typename forward_list_node_value_type<node>::type;
    using node_pointer = NodePtr;
    using begin_node = forward_begin_node<NodePtr>;
    using begin_node_pointer = typename pointer_traits<NodePtr>::template rebind<begin_node>;
    using void_pointer = typename pointer_traits<NodePtr>::template rebind<void>;
    using iter_node_pointer = begin_node_pointer;
    using non_iter_node_pointer = conditional_t<
        is_same<iter_node_pointer, node_pointer>::value,
        begin_node_pointer,
        node_pointer
    >;

    static
    iter_node_pointer
    as_iter_node(
        iter_node_pointer p
    )
    {
        return p;
    }

    static
    iter_node_pointer
    as_iter_node(
        non_iter_node_pointer p
    )
    {
        return static_cast<iter_node_pointer>(static_cast<void_pointer>(p));
    }
};

// FORWARD BEGIN NODE

template <typename NodePtr>
struct forward_begin_node
{
    using pointer = NodePtr;
    using begin_node_pointer = typename pointer_traits<NodePtr>::template rebind<forward_begin_node>;

    pointer next_;

    forward_begin_node():
        next_(nullptr)
    {}

    begin_node_pointer
    next_as_begin()
    const
    {
        return static_cast<begin_node_pointer>(next_);
    }
};

// BEGIN NODE OF

template <typename T, typename VoidPtr>
struct begin_node_of
{
    using type = forward_begin_node<
        typename pointer_traits<VoidPtr>::template rebind<forward_list_node<T, VoidPtr>>
    >;
};

// FORWARD LIST NODE

template <typename T, typename VoidPtr>
struct forward_list_node: public begin_node_of<T, VoidPtr>::type
{
    using value_type = T;
    value_type value_;
};

// FORWARD LIST ITERATOR

template <typename NodePtr>
class forward_list_iterator
{
    using traits = forward_node_traits<NodePtr>;
    using node_pointer = typename traits::node_pointer;
    using begin_node_pointer = typename traits::begin_node_pointer;
    using iter_node_pointer = typename traits::iter_node_pointer;
    using void_pointer = typename traits::void_pointer;

    iter_node_pointer ptr_;

    begin_node_pointer
    get_begin()
    const
    {
        return static_cast<begin_node_pointer>(static_cast<void_pointer>(ptr_));
    }

    node_pointer
    get_unsafe_node_pointer()
    const
    {
        return static_cast<node_pointer>(static_cast<void_pointer>(ptr_));
    }

    explicit
    forward_list_iterator(
        nullptr_t
    )
    noexcept:
        ptr_(nullptr)
    {}

    explicit
    forward_list_iterator(
        begin_node_pointer p
    )
    noexcept:
        ptr_(traits::as_iter_node(p))
    {}

    explicit
    forward_list_iterator(
        node_pointer p
    )
    noexcept:
        ptr_(traits::as_iter_node(p))
    {}

    template <typename, typename> friend class forward_list_facet;
    template <typename, typename> friend class forward_list;
    template <typename> friend class forward_list_const_iterator;

public:
    using iterator_category = forward_iterator_tag;
    using value_type = typename traits::node_value_type;
    using reference = value_type&;
    using difference_type = typename pointer_traits<node_pointer>::difference_type;
    using pointer = typename pointer_traits<node_pointer>::template rebind<value_type>;

    forward_list_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    reference
    operator*()
    const
    {
        return get_unsafe_node_pointer()->value_;
    }

    pointer
    operator->()
    const
    {
        return pointer_traits<pointer>::pointer_to(get_unsafe_node_pointer()->value_);
    }

    forward_list_iterator&
    operator++()
    {
        ptr_ = traits::as_iter_node(ptr_->next_);
        return *this;
    }

    forward_list_iterator
    operator++(int)
    {
        forward_list_iterator t(*this);
        ++(*this);
        return t;
    }

    friend
    bool
    operator==(
        const forward_list_iterator& x,
        const forward_list_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const forward_list_iterator& x,
        const forward_list_iterator& y
    )
    {
        return !(x == y);
    }
};

// FORWARD LIST CONST ITERATOR

template <typename NodeConstPtr>
struct forward_list_const_iterator
{
    static_assert((!is_const<typename pointer_traits<NodeConstPtr>::element_type>::value), "");
    using NodePtr = NodeConstPtr;
    using traits = forward_node_traits<NodePtr>;
    using node = typename traits::node;
    using node_pointer = typename traits::node_pointer;
    using begin_node_pointer = typename traits::begin_node_pointer;
    using iter_node_pointer = typename traits::iter_node_pointer;
    using void_pointer = typename traits::void_pointer;

    iter_node_pointer ptr_;

    begin_node_pointer
    get_begin()
    const
    {
        return static_cast<begin_node_pointer>(static_cast<void_pointer>(ptr_));
    }

    node_pointer
    get_unsafe_node_pointer()
    const
    {
        return static_cast<node_pointer>(static_cast<void_pointer>(ptr_));
    }

    explicit
    forward_list_const_iterator(
        nullptr_t
    )
    noexcept:
        ptr_(nullptr)
    {}

    explicit
    forward_list_const_iterator(
        begin_node_pointer p
    )
    noexcept:
        ptr_(traits::as_iter_node(p))
    {}

    explicit
    forward_list_const_iterator(
        node_pointer p
    )
    noexcept:
        ptr_(traits::as_iter_node(p))
    {}

    template <typename, typename> friend class forward_list_facet;
    template <typename, typename> friend class forward_list;

public:
    using iterator_category = forward_iterator_tag;
    using value_type = typename traits::node_value_type;
    using reference = value_type&;
    using difference_type = typename pointer_traits<node_pointer>::difference_type;
    using pointer = typename pointer_traits<node_pointer>::template rebind<const value_type>;

    forward_list_const_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    forward_list_const_iterator(
        forward_list_iterator<node_pointer> p
    )
    noexcept:
        ptr_(p.ptr_)
    {}

    reference
    operator*()
    const
    {
        return get_unsafe_node_pointer()->value_;
    }

    pointer
    operator->()
    const
    {
        return pointer_traits<pointer>::pointer_to(get_unsafe_node_pointer()->value_);
    }

    forward_list_const_iterator&
    operator++()
    {
        ptr_ = traits::as_iter_node(ptr_->next_);
        return *this;
    }

    forward_list_const_iterator
    operator++(int)
    {
        forward_list_const_iterator t(*this);
        ++(*this);
        return t;
    }

    friend
    bool
    operator==(
        const forward_list_const_iterator& x,
        const forward_list_const_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const forward_list_const_iterator& x,
        const forward_list_const_iterator& y
    )
    {
        return !(x == y);
    }
};

// FORWARD LIST FACET

template <
    typename T,
    typename VoidPtr = void*
>
class forward_list_facet
{
protected:
    using void_pointer = VoidPtr;
    using node = forward_list_node<T, void_pointer>;
    using begin_node = typename begin_node_of<T, void_pointer>::type;
    using node_pointer = typename pointer_traits<void_pointer>::template rebind<node>;
    using begin_node_pointer = typename pointer_traits<void_pointer>::template rebind<begin_node>;

    begin_node before_begin_;

    // Pointer
    begin_node_pointer
    before_begin_pointer()
    noexcept
    {
        return pointer_traits<begin_node_pointer>::pointer_to(before_begin_);
    }

    begin_node_pointer
    before_begin_pointer()
    const noexcept
    {
        begin_node& r = const_cast<begin_node&>(before_begin_);
        return pointer_traits<begin_node_pointer>::pointer_to(r);
    }

    node_pointer&
    begin_pointer()
    noexcept
    {
        return before_begin_pointer()->next_;
    }

    node_pointer&
    begin_pointer()
    const noexcept
    {
        return before_begin_pointer()->next_;
    }

    // Constructors
    forward_list_facet(const forward_list_facet&) = delete;

    forward_list_facet(
        forward_list_facet&& x
    )
    noexcept:
        before_begin_(move(x.before_begin_))
    {
        x.begin_pointer() = nullptr;
    }

    // Assignment
    forward_list_facet& operator=(const forward_list_facet&) = delete;

    forward_list_facet&
    operator=(
        forward_list_facet&& x
    )
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void
    swap(
        forward_list_facet& x
    )
    noexcept
    {
        fast_swap(begin_pointer(), x.begin_pointer());
    }

    template <typename, typename> friend class forward_list;

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename pointer_traits<void_pointer>::template rebind<value_type>;
    using const_pointer = typename pointer_traits<void_pointer>::template rebind<const value_type>;
    using difference_type = typename pointer_traits<void_pointer>::difference_type;
    using size_type = make_unsigned_t<difference_type>;
    using iterator = forward_list_iterator<node_pointer>;
    using const_iterator = forward_list_const_iterator<node_pointer>;

    // Constructors
    forward_list_facet()
    noexcept:
        before_begin_(begin_node())
    {}

    // Element access
    reference
    front()
    {
        return begin_pointer()->value_;
    }

    const_reference
    front()
    const
    {
        return begin_pointer()->value_;
    }

    // Iterators
    iterator
    before_begin()
    noexcept
    {
        return iterator(before_begin_pointer());
    }

    const_iterator
    before_begin()
    const noexcept
    {
        return const_iterator(before_begin_pointer());
    }

    const_iterator
    cbefore_begin()
    const noexcept
    {
        return before_begin();
    }

    iterator
    begin()
    noexcept
    {
        return iterator(begin_pointer());
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(begin_pointer());
    }

    const_iterator
    cbegin()
    const noexcept
    {
        return begin();
    }

    iterator
    end()
    noexcept
    {
        return iterator(nullptr);
    }

    const_iterator
    end()
    const noexcept
    {
        return const_iterator(nullptr);
    }

    const_iterator
    cend()
    const noexcept
    {
        return end();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return begin_pointer() == nullptr;
    }

    size_type
    max_size()
    const noexcept
    {
        // guaranteed to be constexpr
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // Operations
    void
    merge(
        forward_list_facet&& x
    )
    {
        merge(x, less<value_type>());
    }

    template <typename Compare>
    void
    merge(
        forward_list_facet&& x,
        Compare comp
    )
    {
        merge(x, move(comp));
    }

    void
    merge(
        forward_list_facet& x
    )
    {
        merge(x, less<value_type>());
    }

    template <typename Compare>
    void
    merge(
        forward_list_facet& x,
        Compare comp
    )
    {
        if (this != &x) {
            begin_pointer() = merge_list(begin_pointer(), x.begin_pointer(), comp);
            x.begin_pointer() = nullptr;
        }
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet&& x
    )
    {
        splice_after(p, x);
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet&& x,
        const_iterator i
    )
    {
        splice_after(p, x, i);
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet&& x,
        const_iterator f,
        const_iterator l
    )
    {
        splice_after(p, x, f, l);
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet& x
    )
    {
        if (!x.empty()) {
            if (p.get_begin()->next_ != nullptr) {
                const_iterator lml = x.before_begin();
                while (lml.get_begin()->next_ != nullptr) {
                    ++lml;
                }
                lml.get_begin()->next_ = p.get_begin()->next_;
            }
            p.get_begin()->next_ = x.begin_pointer();
            x.begin_pointer() = nullptr;
        }
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet&,
        const_iterator i
    )
    {
        const_iterator lml = std::next(i);
        if (p != i && p != lml) {
            i.get_begin()->next_ = lml.get_begin()->next_;
            lml.get_begin()->next_ = p.get_begin()->next_;
            p.get_begin()->next_ = lml.get_unsafe_node_pointer();
        }
    }

    void
    splice_after(
        const_iterator p,
        forward_list_facet& x,
        const_iterator f,
        const_iterator l
    )
    {
        if (f != l && p != f) {
            const_iterator lm1 = f;
            while (lm1.get_unsafe_node_pointer()->next_ != l.get_unsafe_node_pointer()) {
                ++lm1;
            }
            if (f != lm1) {
                lm1.get_unsafe_node_pointer()->next_ = p.get_unsafe_node_pointer()->next_;
                p.get_unsafe_node_pointer()->next_ = f.get_unsafe_node_pointer()->next_;
                f.get_unsafe_node_pointer()->next_ = l.get_unsafe_node_pointer();
            }
        }
    }

    void
    reverse()
    noexcept
    {
        node_pointer p = before_begin_pointer();
        if (p != nullptr) {
            node_pointer f = p->next_;
            p->next_ = nullptr;
            while (f != nullptr)
            {
                node_pointer t = f->next_;
                f->next_ = p;
                p = f;
                f = t;
            }
            begin_pointer() = p;
        }
    }

    void
    sort()
    {
        sort(less<value_type>());
    }

    template <typename Compare>
    void
    sort(
        Compare comp
    )
    {
        difference_type d = distance(begin(), end());
        begin_pointer() = sort_list(before_begin_pointer(), d, comp);
    }

private:
    template <typename Compare>
    static
    node_pointer
    merge_list(
        node_pointer f1,
        node_pointer f2,
        Compare& comp
    )
    {
        if (f1 == nullptr) {
            return f2;
        }
        if (f2 == nullptr) {
            return f1;
        }
        node_pointer r;
        if (comp(f2->value_, f1->value_)) {
            node_pointer t = f2;
            while (t->next_ != nullptr && comp(t->next_->value_, f1->value_)) {
                t = t->next_;
            }
            r = f2;
            f2 = t->next_;
            t->next_ = f1;
        } else {
            r = f1;
        }

        node_pointer p = f1;
        f1 = f1->next_;
        while (f1 != nullptr && f2 != nullptr) {
            if (comp(f2->value_, f1->value_)) {
                node_pointer t = f2;
                while (t->next_ != nullptr && comp(t->next_->value_, f1->value_)) {
                    t = t->next_;
                }
                p->next_ = f2;
                f2 = t->next_;
                t->next_ = f1;
            }
            p = f1;
            f1 = f1->next_;
        }
        if (f2 != nullptr) {
            p->next_ = f2;
        }
        return r;
    }

    template <typename Compare>
    static
    node_pointer
    sort_list(
        node_pointer f1,
        difference_type d,
        Compare& comp
    )
    {
        switch (d)
        {
            case 0:
            case 1:
                return f1;
            case 2:
                if (comp(f1->next_->value_, f1->value_)) {
                    node_pointer t = f1->next_;
                    t->next_ = f1;
                    f1->next_ = nullptr;
                    f1 = t;
                }
                return f1;
        }

        difference_type d1 = d / 2;
        difference_type d2 = d - d1;
        node_pointer t = std::next(iterator(f1), d1 - 1).get_unsafe_node_pointer();
        node_pointer f2 = t->next_;
        t->next_ = nullptr;

        return merge_list(sort_list(f1, d1, comp), sort_list(f2, d2, comp), comp);
    }
};


template <typename T, typename VoidPtr>
inline
bool
operator==(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    using list_type = forward_list_facet<T, VoidPtr>;
    using iterator_type = typename list_type::const_iterator;

    iterator_type ix = x.begin();
    iterator_type ex = x.end();
    iterator_type iy = y.begin();
    iterator_type ey = y.end();

    for (; ix != ex && iy != ey; ++ix, ++iy) {
        if (!(*ix == *iy)) {
            return false;
        }
    }
    return (ix == ex) == (iy == ey);
}

template <typename T, typename VoidPtr>
inline
bool
operator!=(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    return !(x == y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename VoidPtr>
inline
bool
operator>(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    return y < x;
}

template <typename T, typename VoidPtr>
inline
bool
operator>=(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    return !(x < y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<=(
    const forward_list_facet<T, VoidPtr>& x,
    const forward_list_facet<T, VoidPtr>& y
)
{
    return !(y < x);
}

// FORWARD LIST

template <
    typename T,
    typename Allocator = allocator<T>
>
class forward_list
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using facet_type = forward_list_facet<
        value_type,
        typename allocator_traits<allocator_type>::void_pointer
    >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename facet_type::pointer;
    using const_pointer = typename facet_type::const_pointer;
    using size_type = typename facet_type::size_type;
    using difference_type = typename facet_type::difference_type;
    using iterator = typename facet_type::iterator;
    using const_iterator = typename facet_type::const_iterator;

protected:
    static_assert(
        is_same<typename allocator_type::value_type, value_type>::value,
        "Allocator::value_type must be same type as value_type"
    );

    using alloc_traits = allocator_traits<allocator_type>;
    using node = typename facet_type::node;
    using node_pointer = typename facet_type::node_pointer;
    using node_allocator = typename alloc_traits::template rebind_alloc<node>;
    using node_traits = allocator_traits<node_allocator>;
    using begin_node = typename facet_type::begin_node;
    using begin_node_pointer = typename facet_type::begin_node_pointer;
    using begin_node_allocator = typename alloc_traits::template rebind_alloc<begin_node>;
    using begin_node_traits = allocator_traits<begin_node_allocator>;

public:
    // Constructors
    forward_list()
    noexcept:
        data_()
    {}

    explicit
    forward_list(
        const allocator_type& alloc
    ):
        data_(node_allocator(alloc))
    {}

    explicit
    forward_list(
        size_type n
    ):
        forward_list(n, value_type())
    {}

    forward_list(
        size_type n,
        const value_type& v
    ):
        forward_list(n, v, allocator_type())
    {}

    forward_list(
        size_type n,
        const value_type& v,
        const allocator_type& alloc
    ):
        forward_list(alloc)
    {
        insert_after(cbefore_begin(), n, v);
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    forward_list(
        InputIter f,
        InputIter l
    ):
        forward_list(f, l, allocator_type())
    {}

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    forward_list(
        InputIter f,
        InputIter l,
        const allocator_type& alloc
    ):
        forward_list(alloc)
    {
        insert_after(cbefore_begin(), f, l);
    }


    forward_list(
        const forward_list& x
    ):
        forward_list(x, allocator_type(node_traits::select_on_container_copy_construction(x.alloc())))
    {}

    forward_list(
        const forward_list& x,
        const allocator_type& alloc
    ):
        forward_list(alloc)
    {
        insert_after(cbefore_begin(), x.begin(), x.end());
    }

    forward_list(
        forward_list&& x
    ):
        data_(move(x.facet()))
    {}

    forward_list(
        forward_list&& x,
        const allocator_type& alloc
    ):
        forward_list(alloc)
    {
        if (alloc != x.alloc()) {
            using iter = move_iterator<iterator>;
            insert_after(cbefore_begin(), iter(x.begin()), iter(x.end()));
        } else {
            facet().swap(x.facet());
        }
    }

    forward_list(
        initializer_list<value_type> il
    ):
        forward_list()
    {
        insert_after(cbefore_begin(), il.begin(), il.end());
    }

    forward_list(
        initializer_list<value_type> il,
        const allocator_type& alloc
    ):
        forward_list(alloc)
    {
        insert_after(cbefore_begin(), il.begin(), il.end());
    }

    // Assignment
    forward_list&
    operator=(
        const forward_list& x
    )
    {
        if (this != &x) {
            copy_assign_alloc(x);
            assign(x.begin(), x.end());
        }
        return *this;
    }

    forward_list&
    operator=(
        forward_list&& x
    )
    noexcept
    {
        move_assign(x);
        return *this;
    }

    forward_list&
    operator=(
        initializer_list<value_type> il
    )
    {
        assign(il.begin(), il.end());
        return *this;
    }

    // Destructors
    ~forward_list()
    {
        clear();
    }

    // Assign
    void
    assign(
        size_type n,
        const value_type& v
    )
    {
        iterator i = before_begin();
        iterator j = std::next(i);
        iterator e = end();
        for (; j != e && n > 0; --n, ++i, ++j) {
            *j = v;
        }
        if (j == e) {
            insert_after(i, n, v);
        } else {
            erase_after(i, e);
        }
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    void
    assign(
        InputIter f,
        InputIter l
    )
    {
        iterator i = before_begin();
        iterator j = std::next(i);
        iterator e = end();
        for (; j != e && f != l; ++i, (void) ++j, ++f) {
            *j = *f;
        }
        if (j == e) {
            insert_after(i, f, l);
        } else {
            erase_after(i, e);
        }
    }

    void
    assign(
        initializer_list<T> il
    )
    {
        assign(il.begin(), il.end());
    }

    // Observers
    allocator_type
    get_allocator()
    const noexcept
    {
        return allocator_type(alloc());
    }

    // Element access
    reference
    front()
    {
        return facet().front();
    }

    const_reference
    front()
    const
    {
        return facet().front();
    }

    // Iterators
    iterator
    before_begin()
    noexcept
    {
        return facet().before_begin();
    }

    const_iterator
    before_begin()
    const noexcept
    {
        return facet().before_begin();
    }

    const_iterator
    cbefore_begin()
    const noexcept
    {
        return facet().cbefore_begin();
    }

    iterator
    begin()
    noexcept
    {
        return facet().begin();
    }

    const_iterator
    begin()
    const noexcept
    {
        return facet().begin();
    }

    const_iterator
    cbegin()
    const noexcept
    {
        return facet().cbegin();
    }

    iterator
    end()
    noexcept
    {
        return facet().end();
    }

    const_iterator
    end()
    const noexcept
    {
        return facet().end();
    }

    const_iterator
    cend()
    const noexcept
    {
        return facet().cend();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return facet().empty();
    }

    size_type
    max_size()
    const noexcept
    {
        return facet().max_size();
    }

    // Modifiers
    void
    clear()
    noexcept
    {
        node_allocator& a = alloc();
        for (node_pointer p = facet().begin_pointer(); p != nullptr;) {
            node_pointer next = p->next_;
            node_traits::destroy(a, addressof(p->value_));
            node_traits::deallocate(a, p, 1);
            p = next;
        }
        facet().begin_pointer() = nullptr;
    }

    iterator
    insert_after(
        const_iterator p,
        value_type&& v
    )
    {
        begin_node_pointer const r = p.get_begin();
        node_allocator& a = alloc();
        using deleter = allocator_destructor<node_allocator>;
        unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
        node_traits::construct(a, addressof(h->value_), move(v));
        h->next_ = r->next_;
        r->next_ = h.release();
        return iterator(r->next_);
    }

    iterator
    insert_after(
        const_iterator p,
        const value_type& v
    )
    {
        begin_node_pointer const r = p.get_begin();
        node_allocator& a = alloc();
        using deleter = allocator_destructor<node_allocator>;
        unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
        node_traits::construct(a, addressof(h->value_), v);
        h->next_ = r->next_;
        r->next_ = h.release();
        return iterator(r->next_);
    }

    iterator
    insert_after(
        const_iterator p,
        size_type n,
        const value_type& v
    )
    {
        begin_node_pointer r = p.get_begin();
        if (n > 0) {
            node_allocator& a = alloc();
            using deleter = allocator_destructor<node_allocator>;
            unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
            node_traits::construct(a, addressof(h->value_), v);
            node_pointer first = h.release();
            node_pointer last = first;
            try {
                for (--n; n != 0; --n, last = last->next_) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, addressof(h->value_), v);
                    last->next_ = h.release();
                }
            } catch (...) {
                while (first != nullptr) {
                    node_pointer next = first->next_;
                    node_traits::destroy(a, addressof(first->value_));
                    node_traits::deallocate(a, first, 1);
                    first = next;
                }
                throw;
            }
            last->next_ = r->next_;
            r->next_ = first;
            r = static_cast<begin_node_pointer>(last);
        }
        return iterator(r);
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    iterator
    insert_after(
        const_iterator p,
        InputIter f,
        InputIter l
    )
    {
        begin_node_pointer r = p.get_begin();
        if (f != l) {
            node_allocator& a = alloc();
            using deleter = allocator_destructor<node_allocator>;
            unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
            node_traits::construct(a, addressof(h->value_), *f);
            node_pointer first = h.release();
            node_pointer last = first;
            try {
                for (++f; f != l; ++f, ((void)(last = last->next_))) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, addressof(h->value_), *f);
                    last->next_ = h.release();
                }
            } catch (...) {
                while (first != nullptr) {
                    node_pointer next = first->next_;
                    node_traits::destroy(a, addressof(first->value_));
                    node_traits::deallocate(a, first, 1);
                    first = next;
                }
                throw;
            }
            last->next_ = r->next_;
            r->next_ = first;
            r = static_cast<begin_node_pointer>(last);
        }
        return iterator(r);
    }

    iterator
    insert_after(
        const_iterator p,
        initializer_list<value_type> il
    )
    {
        return insert_after(p, il.begin(), il.end());
    }

    template <typename ... Ts>
    iterator
    emplace_after(
        const_iterator p,
        Ts&&... ts
    )
    {
        begin_node_pointer const r = p.get_begin();
        node_allocator& a = alloc();
        using deleter = allocator_destructor<node_allocator>;
        unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
        node_traits::construct(a, addressof(h->value_), forward<Ts>(ts)...);
        h->next_ = r->next_;
        r->next_ = h.release();
        return iterator(r->next_);
    }

    iterator
    erase_after(
        const_iterator f
    )
    {
        begin_node_pointer p = f.get_begin();
        node_pointer n = p->next_;
        p->next_ = n->next_;
        node_allocator& a = alloc();
        node_traits::destroy(a, addressof(n->value_));
        node_traits::deallocate(a, n, 1);
        return iterator(p->next_);
    }

    iterator
    erase_after(
        const_iterator f,
        const_iterator l
    )
    {
        node_pointer e = l.get_unsafe_node_pointer();
        if (f != l) {
            begin_node_pointer bp = f.get_begin();
            node_pointer n = bp->next_;
            if (n != e) {
                bp->next_ = e;
                node_allocator& a = alloc();
                do {
                    node_pointer tmp = n->next_;
                    node_traits::destroy(a, addressof(n->value_));
                    node_traits::deallocate(a, n, 1);
                    n = tmp;
                } while (n != e);
            }
        }
        return iterator(e);
    }

    void
    push_front(
        const value_type& v
    )
    {
        node_allocator& a = alloc();
        using deleter = allocator_destructor<node_allocator>;
        unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
        node_traits::construct(a, addressof(h->value_), v);
        h->next_ = facet().begin_pointer();
        facet().begin_pointer() = h.release();
    }

    void
    push_front(
        value_type&& v
    )
    {
        emplace_front(forward<value_type>(v));
    }

    template <typename ... Ts>
    reference
    emplace_front(
        Ts&&... ts
    )
    {
        node_allocator& a = alloc();
        using deleter = allocator_destructor<node_allocator>;
        unique_ptr<node, deleter> h(node_traits::allocate(a, 1), deleter(a, 1));
        node_traits::construct(a, addressof(h->value_), forward<Ts>(ts)...);
        h->next_ = facet().begin_pointer();
        facet().begin_pointer() = h.release();
        return facet().begin_pointer()->value_;
    }

    void
    pop_front()
    {
        node_allocator& a = alloc();
        node_pointer p = facet().begin_pointer();
        facet().begin_pointer() = p->next_;
        node_traits::destroy(a, addressof(p->value_));
        node_traits::deallocate(a, p, 1);
    }

    void
    resize(
        size_type n
    )
    {
        resize(n, value_type());
    }

    void
    resize(
        size_type n,
        const value_type& v
    )
    {
        size_type sz = 0;
        iterator p = before_begin();
        iterator i = begin();
        iterator e = end();
        for (; i != e && sz < n; ++p, ++i, ++sz)
            ;
        if (i != e) {
            erase_after(p, e);
        } else {
            n -= sz;
            if (n > 0) {
                node_allocator& a = alloc();
                using deleter = allocator_destructor<node_allocator>;
                unique_ptr<node, deleter> h(nullptr, deleter(a, 1));
                for (begin_node_pointer ptr = p.get_begin(); n > 0; --n, ptr = ptr->next_as_begin()) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, addressof(h->value_), v);
                    h->next_ = nullptr;
                    ptr->next_ = h.release();
                }
            }
        }
    }

    void
    swap(
        forward_list& x
    )
    noexcept
    {
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
    }

    // Operations
    void
    merge(
        forward_list&& x
    )
    {
        facet().merge(move(x.facet()));
    }

    template <typename Compare>
    void
    merge(
        forward_list&& x,
        Compare comp
    )
    {
        facet().merge(move(x.facet()), move(comp));
    }

    void
    merge(
        forward_list& x
    )
    {
        facet().merge(x.facet());
    }

    template <typename Compare>
    void
    merge(
        forward_list& x,
        Compare comp
    )
    {
        facet().merge(x.facet(), move(comp));
    }

    void
    splice_after(
        const_iterator p,
        forward_list&& x
    )
    {
        facet().splice_after(p, move(x.facet()));
    }

    void
    splice_after(
        const_iterator p,
        forward_list&& x,
        const_iterator i
    )
    {
        facet().splice_after(p, move(x.facet()), i);
    }

    void
    splice_after(
        const_iterator p,
        forward_list&& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice_after(p, move(x.facet()), f, l);
    }

    void
    splice_after(
        const_iterator p,
        forward_list& x
    )
    {
        facet().splice_after(p, x.facet());
    }

    void
    splice_after(
        const_iterator p,
        forward_list& x,
        const_iterator i
    )
    {
        facet().splice_after(p, x.facet(), i);
    }

    void
    splice_after(
        const_iterator p,
        forward_list& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice_after(p, x.facet(), f, l);
    }

    void
    remove(
        const_reference v
    )
    {
        remove_if([&v](const_reference value) -> bool {
            return value == v;
        });
    }

    template <typename Predicate>
    void
    remove_if(
        Predicate pred
    )
    {
        iterator e = end();
        for (iterator i = before_begin(); i.get_begin()->next_ != nullptr;)
        {
            if (pred(i.get_begin()->next_->value_)) {
                iterator j = std::next(i, 2);
                for (; j != e && pred(*j); ++j)
                    ;
                erase_after(i, j);
                if (j == e) {
                    break;
                }
                i = j;
            } else {
                ++i;
            }
        }
    }

    void
    reverse()
    noexcept
    {
        facet().reverse();
    }

    void
    unique()
    {
        unique(equal_to<value_type>());
    }

    template <typename Predicate>
    void
    unique(
        Predicate pred
    )
    {
        for (iterator i = begin(), e = end(); i != e;) {
            iterator j = std::next(i);
            for (; j != e && pred(*i, *j); ++j)
                ;
            if (i.get_begin()->next_ != j.get_unsafe_node_pointer()) {
                erase_after(i, j);
            }
            i = j;
        }
    }

    void
    sort()
    {
        facet().sort();
    }

    template <typename Compare>
    void
    sort(
        Compare comp
    )
    {
        facet().sort(move(comp));
    }

    // Facet
    facet_type&
    facet()
    noexcept
    {
        return get<0>(data_);
    }

    const facet_type&
    facet()
    const noexcept
    {
        return get<0>(data_);
    }

private:
    compressed_pair<facet_type, node_allocator> data_;

    // Allocator
    node_allocator&
    alloc()
    noexcept
    {
        return get<1>(data_);
    }

    const node_allocator&
    alloc()
    const noexcept
    {
        return get<1>(data_);
    }

    // Copy Assign Alloc
    void
    copy_assign_alloc(
        const forward_list& x,
        true_type
    )
    {
        if (alloc() != x.alloc()) {
            clear();
        }
        alloc() = x.alloc();
    }

    void
    copy_assign_alloc(
        const forward_list&,
        false_type
    )
    {}

    void
    copy_assign_alloc(
        const forward_list& x
    )
    {
        constexpr bool propagate = node_traits::propagate_on_container_copy_assignment::value;
        copy_assign_alloc(x, integral_constant<bool, propagate>());
    }

    // Move Assign Alloc
    void
    move_assign_alloc(
        forward_list& x,
        true_type
    )
    noexcept
    {
        alloc() = move(x.alloc());
    }

    void
    move_assign_alloc(
        forward_list&,
        false_type
    )
    noexcept
    {}

    void
    move_assign_alloc(
        forward_list& x
    )
    noexcept
    {
        constexpr bool propagate = node_traits::propagate_on_container_move_assignment::value;
        move_assign_alloc(x, integral_constant<bool, propagate>());
    }

    // Move Assign
    void
    move_assign(
        forward_list& x,
        true_type
    )
    {
        clear();
        move_assign_alloc(x);
        facet().begin_pointer() = x.facet().begin_pointer();
        x.facet().begin_pointer() = nullptr;
    }

    void
    move_assign(
        forward_list& x,
        false_type
    )
    {
        if (alloc() == x.alloc()) {
            move_assign(x, true_type());
        } else {
            using iter = move_iterator<iterator>;
            assign(iter(x.begin()), iter(x.end()));
        }
    }

    void
    move_assign(
        forward_list& x
    )
    {
        constexpr bool propagate = node_traits::propagate_on_container_move_assignment::value;
        move_assign(x, integral_constant<bool, propagate>());
    }
};


template <typename T, typename Allocator>
inline
bool
operator==(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() == y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator!=(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() != y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() < y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() > y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>=(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() >= y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<=(
    const forward_list<T, Allocator>& x,
    const forward_list<T, Allocator>& y
)
{
    return x.facet() <= y.facet();
}

template <typename T, typename Allocator>
inline
void
swap(
    forward_list<T, Allocator>& x,
    forward_list<T, Allocator>& y
)
noexcept
{
    x.swap(y);
}

// SPECIALIZATION
// --------------

template <typename NodePtr>
struct is_relocatable<forward_begin_node<NodePtr>>: is_relocatable<NodePtr>
{};

template <typename T, typename VoidPtr>
struct is_relocatable<begin_node_of<T, VoidPtr>>: true_type
{};

template <typename T, typename VoidPtr>
struct is_relocatable<forward_list_node<T, VoidPtr>>: is_relocatable<T>
{};

template <typename NodePtr>
struct is_relocatable<forward_list_iterator<NodePtr>>: is_relocatable<NodePtr>
{};

template <typename NodePtr>
struct is_relocatable<forward_list_const_iterator<NodePtr>>: is_relocatable<NodePtr>
{};

template <typename T, typename VoidPtr>
struct is_relocatable<forward_list_facet<T, VoidPtr>>: is_relocatable<begin_node_of<T, VoidPtr>>
{};

template <typename T, typename Allocator>
struct is_relocatable<forward_list<T, Allocator>>:
    bool_constant<
        is_relocatable<forward_list_facet<T, typename allocator_traits<Allocator>::void_pointer>>::value &&
        is_relocatable<Allocator>::value
    >
{};

PYCPP_END_NAMESPACE
