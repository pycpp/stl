//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL forward_list with allocator erasure from iterators.
 *
 *  \synopsis
 *      # TODO: iterator....
 *
 *      template <typename T, typename Allocator = std::allocator<T>>
 *      class forward_list
 *      {
 *          using value_type = T;
 *          using allocator_type = Allocator;
 *          using reference = value_type&;
 *          using const_reference = const value_type&;
 *          using size_type = typename alloc_traits::size_type;
 *          using difference_type = typename alloc_traits::difference_type;
 *          using pointer = typename alloc_traits::pointer;
 *          using const_pointer = typename alloc_traits::const_pointer;
 *          using iterator = implementation-defined;
 *          using const_iterator = implementation-defined;
 *      };
 *
 *  TODO: finish the synopsis
 */


#include <pycpp/stl/container/compressed_pair.h>
#include <pycpp/stl/memory/allocator_destructor.h>
#include <pycpp/stl/memory/allocator_traits.h>
#include <forward_list>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

namespace stl_detail
{
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
    using node = typename std::remove_cv<
        typename std::pointer_traits<NodePtr>::element_type
    >::type;
    using node_value_type = typename forward_list_node_value_type<node>::type;
    using node_pointer = NodePtr;
    using begin_node = forward_begin_node<NodePtr>;
    using begin_node_pointer = typename std::pointer_traits<NodePtr>::template rebind<begin_node>;
    using void_pointer = typename std::pointer_traits<NodePtr>::template rebind<void>;
    using iter_node_pointer = begin_node_pointer;
    using non_iter_node_pointer = typename std::conditional<
        std::is_same<iter_node_pointer, node_pointer>::value,
        node_pointer,
        begin_node_pointer
    >::type;

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
    using begin_node_pointer = typename std::pointer_traits<NodePtr>::template rebind<forward_begin_node>;

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
        typename std::pointer_traits<VoidPtr>::template rebind<forward_list_node<T, VoidPtr>>
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
    //template <typename, typename> friend class forward_list;
    template <typename> friend class forward_list_const_iterator;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename traits::node_value_type;
    using reference = value_type&;
    using difference_type = typename std::pointer_traits<node_pointer>::difference_type;
    using pointer = typename std::pointer_traits<node_pointer>::template rebind<value_type>;

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
        return std::pointer_traits<pointer>::pointer_to(get_unsafe_node_pointer()->value_);
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
    static_assert((!std::is_const<typename std::pointer_traits<NodeConstPtr>::element_type>::value), "");
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
    //template <typename, typename> friend class forward_list;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename traits::node_value_type;
    using reference = value_type&;
    using difference_type = typename std::pointer_traits<node_pointer>::difference_type;
    using pointer = typename std::pointer_traits<node_pointer>::template rebind<const value_type>;

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
        return std::pointer_traits<pointer>::pointer_to(get_unsafe_node_pointer()->value_);
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
    typename VoidPtr = void*,
    typename Size = size_t,
    typename DiffT = std::ptrdiff_t
>
class forward_list_facet
{
protected:
    using void_pointer = VoidPtr;
    using node = forward_list_node<T, void_pointer>;
    using begin_node = typename begin_node_of<T, void_pointer>::type;
    using node_pointer = typename std::pointer_traits<void_pointer>::template rebind<node>;
    using begin_node_pointer = typename std::pointer_traits<void_pointer>::template rebind<begin_node>;

    begin_node before_begin_;

    forward_list_facet()
    noexcept:
        before_begin_(begin_node())
    {}

    begin_node_pointer
    before_begin_pointer()
    noexcept
    {
        return std::pointer_traits<begin_node_pointer>::pointer_to(before_begin_);
    }

    begin_node_pointer
    before_begin_pointer()
    const noexcept
    {
        begin_node& r = const_cast<begin_node&>(before_begin_);
        return std::pointer_traits<begin_node_pointer>::pointer_to(r);
    }

    begin_node_pointer
    begin_pointer()
    noexcept
    {
        return before_begin_pointer()->next_;
    }

    begin_node_pointer
    begin_pointer()
    const noexcept
    {
        return before_begin_pointer()->next_;
    }

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::pointer_traits<void_pointer>::template rebind<value_type>;
    using const_pointer = typename std::pointer_traits<void_pointer>::template rebind<const value_type>;
    using size_type = Size;
    using difference_type = DiffT;
    using iterator = forward_list_iterator<node_pointer>;
    using const_iterator = forward_list_const_iterator<node_pointer>;

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
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // Operations
    void
    merge(
        forward_list_facet&& x
    )
    {
        merge(x, std::less<value_type>());
    }

    template <typename Compare>
    void
    merge(
        forward_list_facet&& x, _Compare comp
    )
    {
        merge(x, std::move(comp));
    }

    void
    merge(
        forward_list_facet& x
    )
    {
        merge(x, std::less<value_type>());
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
            before_begin_pointer() = p;
        }
    }

    void
    sort()
    {
        sort(std::less<value_type>());
    }

    template <typename Compare>
    void sort(
        Compare comp
    )
    {
        difference_type d = std::distance(begin(), end());
        before_begin_pointer() = sort_list(before_begin_pointer(), d, comp);
    }

private:
    template <typename Compare>
    static
    node_pointer
    merge_list(
        node_pointer f1,
        node_pointer f2,
        Compare comp
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


template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator==(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    using list_type = forward_list forward_list<T, VoidPtr, Size, DiffT>;
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

template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator!=(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    return !(x == y);
}

template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator<(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator>(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    return y < x;
}

template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator>=(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    return !(x < y);
}

template <typename T, typename VoidPtr, typename Size, typename DiffT>
inline
bool
operator<=(
    const forward_list_facet<T, VoidPtr, Size, DiffT>& x,
    const forward_list_facet<T, VoidPtr, Size, DiffT>& y
)
{
    return !(y < x);
}

// FORWARD LIST BASE

// TODO: remove the base...
template <typename T, typename Allocator>
class forward_list_base
{
//protected:
//    using value_type = T;
//    using allocator_type = Allocator;
//    using alloc_traits = allocator_traits<allocator_type>;
//    using void_pointer = typename alloc_traits::void_pointer;
//    using node = forward_list_node<value_type, void_pointer>;
//    using begin_node = typename begin_node_of<value_type, void_pointer>::type;
//    using node_allocator = typename alloc_traits::template rebind_alloc<node>;
//    using node_traits = allocator_traits<node_allocator>;
//    using node_pointer = typename node_traits::pointer;
//    using begin_node_allocator = typename alloc_traits::template rebind_alloc<begin_node>;
//    using begin_node_traits = allocator_traits<begin_node_allocator>;
//    using begin_node_pointer = typename begin_node_traits::pointer;
//
//    compressed_pair<begin_node, node_allocator> before_begin_;
//
//    begin_node_pointer before_begin_impl() noexcept
//    {
//        return std::pointer_traits<begin_node_pointer>::pointer_to(get<0>(before_begin_));
//    }
//
//    begin_node_pointer before_begin_impl() const noexcept
//    {
//        return std::pointer_traits<begin_node_pointer>::pointer_to(const_cast<begin_node&>(get<0>(before_begin_)));
//    }
//
//    node_allocator& alloc() noexcept
//    {
//        return get<1>(before_begin_);
//    }
//
//    const node_allocator& alloc() const noexcept
//    {
//        return get<1>(before_begin_);
//    }
//
//    using iterator = forward_list_iterator<node_pointer>;
//    using const_iterator = forward_list_const_iterator<node_pointer>;
//
//    forward_list_base() noexcept:
//        before_begin_(begin_node())
//    {}
//
//    forward_list_base(const allocator_type& alloc):
//        before_begin_(begin_node(), node_allocator(alloc))
//    {}
//
//public:
//    forward_list_base(const forward_list_base&) = delete;
//    forward_list_base& operator=(const forward_list_base&) = delete;
//
//    forward_list_base(forward_list_base&& x) noexcept:
//        before_begin_(std::move(x.before_begin_))
//    {
//        x.before_begin_impl()->next_ = nullptr;
//    }
//
//    forward_list_base(forward_list_base&& x, const allocator_type& alloc):
//        before_begin_(begin_node(), node_allocator(alloc))
//    {
//        if (alloc() == x.alloc()) {
//            before_begin_impl()->next_ = x.before_begin_impl()->next_;
//            x.before_begin_impl()->next_ = nullptr;
//        }
//    }
//
//    ~forward_list_base()
//    {
//        clear();
//    }
//
//protected:
//    void copy_assign_alloc(const forward_list_base& x)
//    {
//        constexpr bool propagate = node_traits::propagate_on_container_copy_assignment::value;
//        copy_assign_alloc(x, std::integral_constant<bool, propagate>());
//    }
//
//    void move_assign_alloc(forward_list_base& x) noexcept
//    {
//        constexpr bool propagate = node_traits::propagate_on_container_move_assignment::value;
//        move_assign_alloc(x, std::integral_constant<bool, propagate>());
//    }
//
//public:
//    void swap(forward_list_base& x) noexcept
//    {
//        std::swap(alloc(), x.alloc());
//        std::swap(before_begin_impl()->next_, x.before_begin_impl()->next_);
//    }
//
//protected:
//    void clear() noexcept
//    {
//        node_allocator& a = alloc();
//        for (node_pointer p = before_begin_impl()->next_; p != nullptr;) {
//            node_pointer next = p->next_;
//            node_traits::destroy(a, std::addressof(p->value_));
//            node_traits::deallocate(a, p, 1);
//            p = next;
//        }
//        before_begin_impl()->next_ = nullptr;
//    }
//
//private:
//    void copy_assign_alloc(const forward_list_base& x, std::true_type)
//    {
//        if (alloc() != x.alloc()) {
//            clear();
//        }
//        alloc() = x.alloc();
//    }
//
//    void copy_assign_alloc(const forward_list_base&, std::false_type)
//    {}
//
//    void move_assign_alloc(forward_list_base& x, std::true_type) noexcept
//    {
//        alloc() = std::move(x.alloc());
//    }
//
//    void move_assign_alloc(forward_list_base&, std::false_type) noexcept
//    {}
};

// FORWARD LIST


template <typename T, typename Allocator>
class forward_list
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using facet_type = forward_list_facet<
        value_type,
        typename allocator_traits<allocator_type>::void_pointer,
        typename allocator_traits<allocator_type>::size_type,
        typename allocator_traits<allocator_type>::difference_type
    >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename allocator_traits<allocator_type>::pointer;
    using const_pointer = typename allocator_traits<allocator_type>::const_pointer;
    using size_type = typename allocator_traits<allocator_type>::size_type;
    using difference_type = typename allocator_traits<allocator_type>::difference_type;
    using iterator = typename facet_type::iterator;
    using const_iterator = typename facet_type::const_iterator;

protected:
    static_assert(
        std::is_same<typename allocator_type::value_type, value_type>::value,
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
    forward_list() noexcept = default;

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

    // TODO: here

//public:


//

//
//    // Constructors

//
//    explicit forward_list(size_type n, const allocator_type& alloc):
//        base(alloc)
//    {
//        if (n > 0) {
//            node_allocator& alloc = base::__alloc();
//            using deleter = allocator_destructor<node_allocator>;
//            std::unique_ptr<node, deleter> h(nullptr, deleter(alloc, 1));
//            for (begin_node_pointer p = base::before_begin_impl(); n > 0; --n, p = p->next_as_begin()) {
//                h.reset(node_traits::allocate(alloc, 1));
//                node_traits::construct(alloc, std::addressof(h->value_));
//                h->next_ = nullptr;
//                p->next_ = h.release();
//            }
//        }
//    }
//
//    forward_list(size_type n, const value_type& v);
//    forward_list(size_type n, const value_type& v, const allocator_type& alloc);

    // Operations

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
        // TODO: implemnet...
    }

    void
    unique()
    {
        unique(std::equal_to<value_type>());
    }

    template <typename Predicate>
    void
    unique(
        Predicate pred
    )
    {
        // TODO: implemnet...
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
};

}   /* stl_detail */

PYCPP_END_NAMESPACE
