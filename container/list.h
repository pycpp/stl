//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL list with allocator erasure from iterators.
 *
 *  \synopsis
 // TODO: document
 */

#pragma once

#include <pycpp/stl/container/compressed_pair.h>
#include <pycpp/stl/cassert.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, typename VoidPtr>
struct list_node;

template <typename T, typename VoidPtr>
struct list_node_base;

// OBJECTS
// -------

// LIST NODE POINTER TRAITS

template <typename T, typename VoidPtr>
struct list_node_pointer_traits
{
    using node = list_node<T, VoidPtr>;
    using node_pointer = typename pointer_traits<VoidPtr>::template rebind<node>;
    using base = list_node_base<T, VoidPtr>;
    using base_pointer = typename pointer_traits<VoidPtr>::template rebind<base>;
    using link_pointer = base_pointer;
    using non_link_pointer = conditional_t<
        is_same<link_pointer, node_pointer>::value,
        base_pointer,
        node_pointer
    >;

    static
    link_pointer
    unsafe_link_pointer_cast(
        link_pointer p
    )
    {
        return p;
    }

    static
    link_pointer
    unsafe_link_pointer_cast(
        non_link_pointer p
    )
    {
        return static_cast<link_pointer>(static_cast<VoidPtr>(p));
    }
};

// LIST NODE BASE

template <typename T, typename VoidPtr>
struct list_node_base
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using node_pointer = typename node_traits::node_pointer;
    using base_pointer = typename node_traits::base_pointer;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer prev_;
    link_pointer next_;

    list_node_base():
        prev_(node_traits::unsafe_link_pointer_cast(self())),
        next_(node_traits::unsafe_link_pointer_cast(self()))
    {}

    base_pointer
    self()
    {
        return pointer_traits<base_pointer>::pointer_to(*this);
    }

    node_pointer
    as_node()
    {
        return static_cast<node_pointer>(self());
    }
};

// LIST NODE

template <typename T, typename VoidPtr>
struct list_node: list_node_base<T, VoidPtr>
{
    T value_;

    using base = list_node_base<T, VoidPtr>;
    using link_pointer = typename base::link_pointer;

    link_pointer as_link()
    {
        return static_cast<link_pointer>(base::self());
    }
};

// LIST ITERATOR

template <typename T, typename VoidPtr>
class list_iterator
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer ptr_;

    explicit
    list_iterator(
        link_pointer p
    )
    noexcept:
        ptr_(p)
    {}

    template <typename, typename> friend class list_facet;
    template <typename, typename> friend class list;
    template <typename, typename> friend class list_const_iterator;

public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using reference = value_type&;
    using pointer = typename pointer_traits<VoidPtr>::template rebind<value_type>;
    using difference_type = typename pointer_traits<pointer>::difference_type;

    list_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    reference
    operator*()
    const
    {
        return ptr_->as_node()->value_;
    }

    pointer
    operator->()
    const
    {
        return pointer_traits<pointer>::pointer_to(ptr_->as_node()->value_);
    }

    list_iterator&
    operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    list_iterator
    operator++(int)
    {
        list_iterator t(*this);
        ++(*this);
        return t;
    }

    list_iterator&
    operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    list_iterator
    operator--(int)
    {
        list_iterator t(*this);
        --(*this);
        return t;
    }

    friend
    bool
    operator==(
        const list_iterator& x,
        const list_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const list_iterator& x,
        const list_iterator& y
    )
    {
        return !(x == y);
    }
};

// LIST CONST ITERATOR

template <typename T, typename VoidPtr>
class list_const_iterator
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer ptr_;

    explicit
    list_const_iterator(
        link_pointer p
    )
    noexcept:
        ptr_(p)
    {}

    template <typename, typename> friend class list_facet;
    template <typename, typename> friend class list;

public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using reference = value_type&;
    using pointer = typename pointer_traits<VoidPtr>::template rebind<const value_type>;
    using difference_type = typename pointer_traits<pointer>::difference_type;

    list_const_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    list_const_iterator(
        const list_iterator<T, VoidPtr>& p
    )
    noexcept:
        ptr_(p.ptr_)
    {}

    reference
    operator*()
    const
    {
        return ptr_->as_node()->value_;
    }

    pointer
    operator->()
    const
    {
        return pointer_traits<pointer>::pointer_to(ptr_->as_node()->value_);
    }

    list_const_iterator&
    operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    list_const_iterator
    operator++(int)
    {
        list_const_iterator t(*this);
        ++(*this);
        return t;
    }

    list_const_iterator&
    operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    list_const_iterator
    operator--(int)
    {
        list_const_iterator t(*this);
        --(*this);
        return t;
    }

    friend
    bool
    operator==(
        const list_const_iterator& x,
        const list_const_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const list_const_iterator& x,
        const list_const_iterator& y
    )
    {
        return !(x == y);
    }
};

// LIST FACET

template <
    typename T,
    typename VoidPtr = void*
>
class list_facet
{
protected:
    using void_pointer = VoidPtr;
    using node = list_node<T, void_pointer>;
    using node_base = list_node_base<T, void_pointer>;
    using node_pointer = typename pointer_traits<void_pointer>::template rebind<node>;
    using node_base_pointer = typename pointer_traits<void_pointer>::template rebind<node_base>;
    using node_pointer_traits = list_node_pointer_traits<T, void_pointer>;
    using link_pointer = typename node_pointer_traits::link_pointer;

    link_pointer
    end_as_link()
    const noexcept
    {
        return node_pointer_traits::unsafe_link_pointer_cast(const_cast<node_base&>(end_).self());
    }

    node_base_pointer
    begin_node_base()
    noexcept
    {
        return end_.next_;
    }

    node_base_pointer
    begin_node_base()
    const noexcept
    {
        node_base& r = const_cast<node_base&>(end_);
        return r.next_;
    }

    node_pointer
    begin_node()
    noexcept
    {
        return begin_node_base()->as_node();
    }

    node_pointer
    begin_node()
    const noexcept
    {
        return begin_node_base()->as_node();
    }

    node_base_pointer
    rbegin_node_base()
    noexcept
    {
        return end_.prev_;
    }

    node_base_pointer
    rbegin_node_base()
    const noexcept
    {
        node_base& r = const_cast<node_base&>(end_);
        return r.prev_;
    }

    node_pointer
    rbegin_node()
    noexcept
    {
        return rbegin_node_base()->as_node();
    }

    node_pointer
    rbegin_node()
    const noexcept
    {
        return rbegin_node_base()->as_node();
    }

    // Constructors
    list_facet(const list_facet&) = delete;

// TODO: fix this move
//    list_facet(
//        list_facet&& x
//    )
//    noexcept:
//        end_(move(x.end_)),
//        size_(move(x.size_))
//    {
//        x.end_ = node_base();
//        x.size_ = 0;
//    }

    // Assignment
    list_facet& operator=(const list_facet&) = delete;

    list_facet&
    operator=(
        list_facet&& x
    )
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void
    swap(
        list_facet& x
    )
    noexcept
    {
        fast_swap(size_, x.size_);
        fast_swap(end_, x.end_);
        if (size_() == 0) {
            end_.next_ = end_.prev_ = end_as_link();
        } else {
            end_.prev_->next_ = end_.next_->prev_ = end_as_link();
        }
        if (x.size_() == 0) {
            x.end_.next_ = x.end_.prev_ = x.end_as_link();
        } else {
            x.end_.prev_->next_ = x.end_.next_->prev_ = x.end_as_link();
        }
    }

    template <typename, typename> friend class list;

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename pointer_traits<void_pointer>::template rebind<value_type>;
    using const_pointer = typename pointer_traits<void_pointer>::template rebind<const value_type>;
    using difference_type = typename pointer_traits<void_pointer>::difference_type;
    using size_type = make_unsigned_t<difference_type>;
    using iterator = list_iterator<value_type, void_pointer>;
    using const_iterator = list_const_iterator<value_type, void_pointer>;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // Constructors
    list_facet()
    noexcept:
        size_(0)
    {}

    // Element access
    reference
    front()
    {
        return begin_node()->value_;
    }

    const_reference
    front()
    const
    {
        return begin_node()->value_;
    }

    reference
    back()
    {
        return rbegin_node()->value_;
    }

    const_reference
    back()
    const
    {
        return rbegin_node()->value_;
    }

    // Iterators
    iterator
    begin()
    noexcept
    {
        return iterator(begin_node_base());
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(begin_node_base());
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
        return iterator(end_as_link());
    }

    const_iterator
    end()
    const noexcept
    {
        return const_iterator(end_as_link());
    }

    const_iterator
    cend()
    const noexcept
    {
        return end();
    }

    reverse_iterator
    rbegin()
    noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator
    rbegin()
    const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator
    crbegin()
    const noexcept
    {
        return rbegin();
    }

    reverse_iterator
    rend()
    noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator
    rend()
    const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator
    crend()
    const noexcept
    {
        return rend();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return size() == 0;
    }

    size_type
    size()
    const noexcept
    {
        return size_;
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
        list_facet&& x
    )
    {
        merge(x, less<value_type>());
    }

    template <typename Compare>
    void
    merge(
        list_facet&& x,
        Compare comp
    )
    {
        merge(x, move(comp));
    }

    void
    merge(
        list_facet& x
    )
    {
        merge(x, less<value_type>());
    }

    template <typename Compare>
    void
    merge(
        list_facet& x,
        Compare comp
    )
    {
        if (this != &x) {
            merge_list(x, comp);
        }
    }

    void
    splice(
        const_iterator p,
        list_facet& x
    )
    {
        assert(this != &x && "list::splice(iterator, list) called with this == &list");
        if (!x.empty()) {
            link_pointer f = x.begin_node_base();
            link_pointer l = x.rbegin_node_base();
            unlink_nodes(f, l);
            link_nodes(p.ptr_, f, l);
            size_ += x.size();
            x.size_ = 0;
        }
    }

    void
    splice(
        const_iterator p,
        list_facet&& x
    )
    {
        splice(p, x);
    }

    void
    splice(
        const_iterator p,
        list_facet&& x,
        const_iterator i
    )
    {
        splice(p, x, i);
    }

    void
    splice(
        const_iterator p,
        list_facet&& x,
        const_iterator f,
        const_iterator l
    )
    {
        splice(p, x, f, l);
    }

    void splice(
        const_iterator p,
        list_facet& x,
        const_iterator i
    )
    {
        if (p.ptr_ != i.ptr_ && p.ptr_ != i.ptr_->next_) {
            link_pointer f = i.ptr_;
            unlink_nodes(f, f);
            link_nodes(p.ptr_, f, f);
            --x.size_;
            ++size_;
        }
    }

    void
    splice(
        const_iterator p,
        list_facet& x,
        const_iterator f,
        const_iterator l
    )
    {
        if (f != l) {
            link_pointer first = f.ptr_;
            --l;
            link_pointer last = l.ptr_;
            if (this != &x) {
                size_type s = distance(f, l) + 1;
                x.size_ -= s;
                size_ += s;
            }
            unlink_nodes(first, last);
            link_nodes(p.ptr_, first, last);
        }
    }

    void
    reverse()
    noexcept
    {
        if (size() > 1) {
            iterator e = end();
            for (iterator i = begin(); i.ptr_ != e.ptr_;) {
                fast_swap(i.ptr_->prev_, i.ptr_->next_);
                i.ptr_ = i.ptr_->prev_;
            }
            fast_swap(e.ptr_->prev_, e.ptr_->next_);
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
        std::sort(begin(), end(), size(), comp);
    }

private:
    node_base end_;
    size_type size_;

    static
    void
    unlink_nodes(
        link_pointer f,
        link_pointer l
    )
    noexcept
    {
        f->prev_->next_ = l->next_;
        l->next_->prev_ = f->prev_;
    }

    static
    void
    link_nodes(
        link_pointer p,
        link_pointer f,
        link_pointer l
    )
    noexcept
    {
        p->prev_->next_ = f;
        f->prev_ = p->prev_;
        p->prev_ = l;
        l->next_ = p;
    }

    void
    link_nodes_at_front(
        link_pointer f,
        link_pointer l
    )
    noexcept
    {
        f->prev_ = end_as_link();
        l->next_ = end_.next_;
        l->next_->prev_ = l;
        end_.next_ = f;
    }

    void
    link_nodes_at_back(
        link_pointer f,
        link_pointer l
    )
    noexcept
    {
        l->next_ = end_as_link();
        f->prev_ = end_.prev_;
        f->prev_->next_ = f;
        end_.prev_ = l;
    }

    template <typename Compare>
    void
    merge_list(
        list_facet& x,
        Compare& comp
    )
    {
        iterator f1 = begin();
        iterator e1 = end();
        iterator f2 = x.begin();
        iterator e2 = x.end();
        while (f1 != e1 && f2 != e2) {
            if (comp(*f2, *f1)) {
                size_type ds = 1;
                iterator m2 = std::next(f2);
                for (; m2 != e2 && comp(*m2, *f1); ++m2, ++ds)
                    ;
                size() += ds;
                x.size() -= ds;
                link_pointer f = f2.ptr_;
                link_pointer l = m2.ptr_->prev_;
                f2 = m2;
                unlink_nodes(f, l);
                m2 = std::next(f1);
                link_nodes(f1.ptr_, f, l);
                f1 = m2;
            } else {
                ++f1;
            }
        }
        splice(e1, x);
    }
};

template <typename T, typename VoidPtr>
inline
bool
operator==(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <typename T, typename VoidPtr>
inline
bool
operator!=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(x == y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename VoidPtr>
inline
bool
operator>(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return y < x;
}

template <typename T, typename VoidPtr>
inline
bool
operator>=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(x < y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(y < x);
}

// LIST

template <
    typename T,
    typename Allocator = allocator<T>
>
class list
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using facet_type = list_facet<
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
    using reverse_iterator = typename facet_type::reverse_iterator;
    using const_reverse_iterator = typename facet_type::const_reverse_iterator;

    // TODO: here...

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
    using node_base = typename facet_type::node_base;
    using node_base_pointer = typename facet_type::node_base_pointer;
    using node_base_allocator = typename alloc_traits::template rebind_alloc<node_base>;
    using node_base_traits = allocator_traits<node_base_allocator>;
    using link_pointer = typename facet_type::link_pointer;

public:
    // TODO: here... node_allocator

    // Constructors
    // TODO:

    // Assignment
    // TODO:

    // Destructors
    ~list()
    {
        clear();
    }

    // Assign
    // TODO:

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

    reference
    back()
    {
        return facet().back();
    }

    const_reference
    back()
    const
    {
        return facet().back();
    }

    // Iterators
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

    iterator
    rbegin()
    noexcept
    {
        return facet().rbegin();
    }

    const_iterator
    rbegin()
    const noexcept
    {
        return facet().rbegin();
    }

    const_iterator
    crbegin()
    const noexcept
    {
        return facet().crbegin();
    }

    iterator
    rend()
    noexcept
    {
        return facet().rend();
    }

    const_iterator
    rend()
    const noexcept
    {
        return facet().rend();
    }

    const_iterator
    crend()
    const noexcept
    {
        return facet().crend();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return facet().empty();
    }

    size_type
    size()
    const noexcept
    {
        return facet().max_size();
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
        if (!empty()) {
            node_allocator& a = alloc();
            link_pointer f = facet().begin_node_base();
            link_pointer l = facet().end_as_link();
            facet_type::unlink_nodes(f, l->prev_);
            facet().size_ = 0;
            while (f != l) {
                node_pointer np = f->as_node();
                f = f->next_;
                node_traits::destroy(a, addressof(np->value_));
                node_traits::deallocate(a, np, 1);
            }
        }
    }

    // TODO: need modifiers

    void
    swap(
        list& x
    )
    noexcept
    {
        using PYCPP_NAMESPACE::swap;
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
    }

    // Operations
    void
    merge(
        list&& x
    )
    {
        facet().merge(move(x.facet()));
    }

    template <typename Compare>
    void
    merge(
        list&& x,
        Compare comp
    )
    {
        facet().merge(move(x.facet()), move(comp));
    }

    void
    merge(
        list& x
    )
    {
        facet().merge(x.facet());
    }

    template <typename Compare>
    void
    merge(
        list& x,
        Compare comp
    )
    {
        facet().merge(x.facet(), move(comp));
    }

    void
    splice(
        const_iterator p,
        list& x
    )
    {
        facet().splice(p, x.facet());
    }

    void
    splice(
        const_iterator p,
        list&& x
    )
    {
        facet().splice(p, move(x.facet()));
    }

    void
    splice(
        const_iterator p,
        list&& x,
        const_iterator i
    )
    {
        facet().splice(p, move(x.facet()), i);
    }

    void
    splice(
        const_iterator p,
        list&& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice(p, move(x.facet()), f, l);
    }

    void splice(
        const_iterator p,
        list& x,
        const_iterator i
    )
    {
        facet().splice(p, x.facet(), i);
    }

    void
    splice(
        const_iterator p,
        list& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice(p, x.facet(), f, l);
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
        for (iterator i = begin(), e = end(); i != e;) {
            if (pred(*i)) {
                iterator j = std::next(i);
                for (; j != e && pred(*j); ++j)
                    ;
                i = erase(i, j);
                if (i != e) {
                    ++i;
                }
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
            if (++i != j) {
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

    // TODO: add here
};

template <typename T, typename Allocator>
inline
bool
operator==(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() == y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator!=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() != y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() < y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() > y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() >= y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() <= y.facet();
}

template <typename T, typename Allocator>
inline
void
swap(
    list<T, Allocator>& x,
    list<T, Allocator>& y
)
noexcept
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
