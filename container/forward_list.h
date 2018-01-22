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

    template <typename, typename> friend class forward_list;
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

    template <typename, typename> friend class forward_list;

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

template <typename T, typename VoidPtr = void*>
class forward_list_facet
{
protected:
    using void_pointer = VoidPtr;
    using node = forward_list_node<T, void_pointer>;
    using begin_node = typename begin_node_of<T, void_pointer>::type;
    using node_allocator = typename alloc_traits::template rebind_alloc<node>;
    using node_traits = allocator_traits<node_allocator>;
    using node_pointer = typename node_traits::pointer;
    using begin_node_allocator = typename alloc_traits::template rebind_alloc<begin_node>;
    using begin_node_traits = allocator_traits<begin_node_allocator>;
    using begin_node_pointer = typename begin_node_traits::pointer;

    begin_node before_begin_;

    forward_list_facet()
    noexcept:
        before_begin_(begin_node())
    {}

public:
    using value_type = T;
    // TODO: here

// TODO: here...
//    begin_node_pointer before_begin() noexcept
//    {
//        return std::pointer_traits<begin_node_pointer>::pointer_to(get<0>(before_begin_));
//    }
//
//    begin_node_pointer before_begin() const noexcept
//    {
//        return std::pointer_traits<begin_node_pointer>::pointer_to(const_cast<begin_node&>(get<0>(before_begin_)));
//    }
};

// TODO: this doesn't actually work...
// I need to re-factor it to remove the base....


// FORWARD LIST BASE

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
class forward_list: private forward_list_base<T, Allocator>
{
// TODO: restore
//    using base = forward_list_base<T, Allocator>;
//    using node_allocator = typename base::node_allocator;
//    using node = typename base::node;
//    using node_traits = typename base::node_traits;
//    using node_pointer = typename base::node_pointer;
//    using begin_node_pointer = typename base::begin_node_pointer;
//
//public:
//    using value_type = T;
//    using allocator_type = Allocator;
//    using reference = value_type&;
//    using const_reference = const value_type&;
//    using pointer = typename allocator_traits<allocator_type>::pointer;
//    using const_pointer = typename allocator_traits<allocator_type>::const_pointer;
//    using size_type = typename allocator_traits<allocator_type>::size_type;
//    using difference_type = typename allocator_traits<allocator_type>::difference_type;
//    using iterator = typename base::iterator;
//    using const_iterator = typename base::const_iterator;
//
//    static_assert(
//        std::is_same<typename allocator_type::value_type, value_type>::value,
//        "Allocator::value_type must be same type as value_type"
//    );
//
//    // Constructors
//    forward_list() noexcept = default;
//
//    explicit forward_list(const allocator_type& alloc):
//        base(alloc)
//    {}
//
//    explicit forward_list(size_type n):
//        forward_list(n, allocator_type())
//    {}
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
//                node_traits::construct(alloc, std::addressof(h->__value_));
//                h->__next_ = nullptr;
//                p->__next_ = h.release();
//            }
//        }
//    }
//
//    forward_list(size_type n, const value_type& v);
//    forward_list(size_type n, const value_type& v, const allocator_type& alloc);
};

}   /* stl_detail */

PYCPP_END_NAMESPACE
