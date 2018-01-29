//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief STL deque with allocator erasure from iterators.
 *
 *  \synopsis
 // TODO: implement
 */

#pragma once

#include <pycpp/preprocessor/compiler_traits.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/container/split_buffer.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#ifndef PYCPP_DEQUE_BLOCK_SIZE
#   define PYCPP_DEQUE_BLOCK_SIZE(T) sizeof(T) < 256 ? 4096 / sizeof(T) : 16
#endif

// Buffer growth factor of 2.0 is the worst possible.
//      https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md#memory-handling
#ifndef PYCPP_DEQUE_GROWTH_FACTOR_NUMERATOR
#   define PYCPP_DEQUE_GROWTH_FACTOR_NUMERATOR 3
#endif

#ifndef PYCPP_DEQUE_GROWTH_FACTOR_DENOMINATOR
#   define PYCPP_DEQUE_GROWTH_FACTOR_DENOMINATOR 2
#endif

// OBJECTS
// -------

// DEQUE ITERATOR

template <typename Pointer, size_t DequeBlockSize>
class deque_iterator
{
public:
    static constexpr size_t block_size = DequeBlockSize;

    using traits = pointer_traits<Pointer>;
    using value_type = typename traits::element_type;
    using reference = value_type&;
    using pointer = Pointer;
    using difference_type = typename traits::difference_type;
    using map_pointer = typename traits::template rebind<pointer>;
    using iterator_category = random_access_iterator_tag;

    // Constructors
    deque_iterator()
    noexcept:
        iter_(nullptr),
        ptr_(nullptr)
    {}

    template <typename P1, enable_if_t<is_convertible<P1, pointer>::value>* = nullptr>
    deque_iterator(
        const deque_iterator<P1, block_size>& it
    )
    noexcept:
        iter_(it.iter_),
        ptr_(it.ptr_)
    {}

    // Operators
    reference
    operator*()
    const
    {
        return *ptr_;
    }

    pointer
    operator->()
    const
    {
        return ptr_;
    }

    deque_iterator&
    operator++()
    {
        if (++ptr_ - *iter_ == block_size) {
            ++iter_;
            ptr_ = *iter_;
        }
        return *this;
    }

    deque_iterator
    operator++(int)
    {
        deque_iterator t(*this);
        ++(*this);
        return t;
    }

    deque_iterator&
    operator--()
    {
        if (ptr_ == *iter_) {
            --iter_;
            ptr_ = *iter_ + block_size;
        }
        --ptr_;
        return *this;
    }

    deque_iterator
    operator--(int)
    {
        deque_iterator t(*this);
        --(*this);
        return t;
    }

    deque_iterator&
    operator+=(
        difference_type n
    )
    {
        if (n != 0) {
            n += ptr_ - *iter_;
            if (n > 0) {
                iter_ += n / block_size;
                ptr_ = *iter_ + n % block_size;
            } else {
                difference_type z = block_size - 1 - n;
                iter_ -= z / block_size;
                ptr_ = *iter_ + (block_size - 1 - z % block_size);
            }
        }
        return *this;
    }

    deque_iterator&
    operator-=(
        difference_type n
    )
    {
        return *this += -n;
    }

    deque_iterator
    operator+(
        difference_type n
    )
    const
    {
        deque_iterator t(*this);
        t += n;
        return t;
    }

    deque_iterator
    operator-(
        difference_type n
    )
    const
    {
        deque_iterator t(*this);
        t -= n;
        return t;
    }

    reference
    operator[](
        difference_type n
    )
    const
    {
        return *(*this + n);
    }

    // TODO: need a lot more operators...

private:
    pointer ptr_;
    map_pointer iter_;

    template <typename, typename, size_t> friend class deque_facet;
    template <typename, typename, size_t, intmax_t, intmax_t> friend class deque;

    // Constructors
    deque_iterator(
        map_pointer m,
        pointer p
    )
    noexcept:
        iter_(m),
        ptr_(p)
    {}
};

// TODO: initialize block_size outside of class

// DEQUE FACET

template <
    typename T,
    typename VoidPtr = void*,
    size_t DequeBlockSize = PYCPP_DEQUE_BLOCK_SIZE(T)
>
class deque_facet
{
public:
    static constexpr size_t block_size = DequeBlockSize;

    using value_type = T;
    using buffer_type = split_buffer_facet<T, VoidPtr>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename pointer_traits<VoidPtr>::template rebind<value_type>;
    using const_pointer = typename pointer_traits<VoidPtr>::template rebind<const value_type>;
    using difference_type = typename pointer_traits<VoidPtr>::difference_type;
    using size_type = make_unsigned_t<difference_type>;
    using iterator = deque_iterator<pointer, block_size>;
    using const_iterator = deque_iterator<const_pointer, block_size>;
    using reverse_iterator = PYSTD::reverse_iterator<iterator>;
    using const_reverse_iterator = PYSTD::reverse_iterator<const_iterator>;
    // TODO: will need to store a reference to the split_buffer's facet.
    // Disable copy assignment, allow moving? Maybe? Private then...
    // Yep, private sounds gravy...

    // TODO: Restore later...
    deque_facet() = delete;
    deque_facet(const deque_facet&) = delete;
    deque_facet& operator=(const deque_facet&) = delete;
    deque_facet(deque_facet&) = delete;
    deque_facet& operator=(deque_facet&) = delete;

    // Iterators
    iterator
    begin()
    noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<pointer>;
        map_pointer mp = map_.begin() + start_ / block_size;
        return iterator(mp, map_.empty() ? 0 : *mp + start_ % block_size);
    }

    const_iterator
    begin()
    const noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<const_pointer>;
        map_pointer mp = static_cast<map_pointer>(map_.begin() + start_ / block_size);
        return const_iterator(mp, map_.empty() ? 0 : *mp + start_ % block_size);
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
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<pointer>;
        size_type p = size() + start_;
        map_pointer mp = map_.begin() + p / block_size;
        return iterator(mp, map_.empty() ? 0 : *mp + p % block_size);
    }

    const_iterator
    end()
    const noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<const_pointer>;
        size_type p = size() + start_;
        map_pointer mp = static_cast<map_pointer>(map_.begin() + p / block_size);
        return iterator(mp, map_.empty() ? 0 : *mp + p % block_size);
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

    // Element access
    reference
    at(
        size_type n
    )
    {
        if (n >= size()) {
            throw out_of_range("vector");
        }
        return (*this)[n];
    }

    const_reference
    at(
        size_type n
    ) const
    {
        if (n >= size()) {
            throw out_of_range("vector");
        }
        return (*this)[n];
    }

    reference
    operator[](
        size_type n
    )
    {
        return begin()[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        return begin()[n];
    }

    reference
    front()
    {
        assert(!empty() && "front() called for empty vector");
        return *begin();
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "front() called for empty vector");
        return *begin();
    }

    reference
    back()
    {
        assert(!empty() && "back() called for empty vector");
        return *rbegin();
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "back() called for empty vector");
        return *rbegin();
    }

    // Capacity
    PYCPP_CPP17_NODISCARD
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

    // TODO:
    // add more methods

private:
    using map_type = split_buffer_facet<pointer, VoidPtr>;

    map_type& map_;
    size_type start_;
    size_type size_;
};

// TODO: initialize block_size outside of class

// TODO: implement...

// DEQUE

template <
    typename T,
    typename Allocator = allocator<T>,
    size_t DequeBlockSize = PYCPP_DEQUE_BLOCK_SIZE(T),
    intmax_t GrowthFactorNumerator = PYCPP_DEQUE_GROWTH_FACTOR_NUMERATOR,
    intmax_t GrowthFactorDenominator = PYCPP_DEQUE_GROWTH_FACTOR_DENOMINATOR
>
struct deque
{
    static constexpr size_t block_size = DequeBlockSize;

    using value_type = T;
    using growth_factor = ratio<GrowthFactorNumerator, GrowthFactorDenominator>;
    using allocator_type = Allocator;
    using facet_type = deque_facet<
        value_type,
        typename allocator_traits<allocator_type>::void_pointer,
        block_size
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

    // Constructors
    // TODO:

    // Iterators
    iterator
    begin()
    noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<pointer>;
        map_pointer mp = map_.begin() + start_ / block_size;
        return iterator(mp, map_.empty() ? 0 : *mp + start_ % block_size);
    }

    const_iterator
    begin()
    const noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<const_pointer>;
        map_pointer mp = static_cast<map_pointer>(map_.begin() + start_ / block_size);
        return const_iterator(mp, map_.empty() ? 0 : *mp + start_ % block_size);
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
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<pointer>;
        size_type p = size() + start_;
        map_pointer mp = map_.begin() + p / block_size;
        return iterator(mp, map_.empty() ? 0 : *mp + p % block_size);
    }

    const_iterator
    end()
    const noexcept
    {
        using traits = pointer_traits<pointer>;
        using map_pointer = typename traits::template rebind<const_pointer>;
        size_type p = size() + start_;
        map_pointer mp = static_cast<map_pointer>(map_.begin() + p / block_size);
        return iterator(mp, map_.empty() ? 0 : *mp + p % block_size);
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

    // Element access
    reference
    at(
        size_type n
    )
    {
        if (n >= size()) {
            throw out_of_range("vector");
        }
        return (*this)[n];
    }

    const_reference
    at(
        size_type n
    ) const
    {
        if (n >= size()) {
            throw out_of_range("vector");
        }
        return (*this)[n];
    }

    reference
    operator[](
        size_type n
    )
    {
        return begin()[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        return begin()[n];
    }

    reference
    front()
    {
        assert(!empty() && "front() called for empty vector");
        return *begin();
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "front() called for empty vector");
        return *begin();
    }

    reference
    back()
    {
        assert(!empty() && "back() called for empty vector");
        return *rbegin();
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "back() called for empty vector");
        return *rbegin();
    }
    // TODO: here..

    // Capacity
    PYCPP_CPP17_NODISCARD
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
        return get<0>(size_);
    }

    size_type
    max_size()
    const noexcept
    {
        // guaranteed to be constexpr
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

// TODO: implement...
//    void resize(size_type n);
//    void resize(size_type n, const value_type& __v);
//    void shrink_to_fit() _NOEXCEPT;

private:
    using alloc_traits = allocator_traits<allocator_type>;
    using pointer_allocator = typename alloc_traits::template rebind_alloc<pointer>;
    using map_type = split_buffer<pointer, growth_factor::num, growth_factor::den, pointer_allocator&>;

    map_type map_;
    size_type start_;
    compressed_pair<size_type, allocator_type> size_;
};

// TODO: initialize block_size outside of class

// DEQUE

// typedef split_buffer<pointer, pointer_allocator> map;
// Uses split buffer...
// Split buffer should likely have a growth size... LOLS....

// SPECIALIZATION
// --------------

template <typename Pointer, size_t DequeBlockSize>
struct is_relocatable<deque_iterator<Pointer, DequeBlockSize>>: is_relocatable<Pointer>
{};

// Stores an internal reference.
template <typename T, typename VoidPtr, size_t DequeBlockSize>
struct is_relocatable<deque_facet<T, VoidPtr, DequeBlockSize>>: false_type
{};

// split_buffer stores internal references.
// References have no guaranteed storage duration.
template <
    typename T,
    typename Allocator,
    size_t DequeBlockSize,
    intmax_t GrowthFactorNumerator,
    intmax_t GrowthFactorDenominator
>
struct is_relocatable<deque<T, Allocator, DequeBlockSize, GrowthFactorNumerator, GrowthFactorDenominator>>: false_type
{};

PYCPP_END_NAMESPACE
