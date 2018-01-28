//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL vector with allocator erasure from iterators.
 *
 *  Uses relocation for relocatable types for better performance.
 *
 *  \synopsis
 // TODO: implement
 */

#pragma once

#include <pycpp/preprocessor/compiler_traits.h>
#include <pycpp/stl/cassert.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/container/split_buffer.h>
// TODO: need functional
// TODO: remove std::

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// Vector growth of 2.0 is the worst possible.
//      https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md#memory-handling
#ifndef PYCPP_VECTOR_GROWTH_FACTOR_NUMERATOR
#   define PYCPP_VECTOR_GROWTH_FACTOR_NUMERATOR 3
#endif

#ifndef PYCPP_VECTOR_GROWTH_FACTOR_DENOMINATOR
#   define PYCPP_VECTOR_GROWTH_FACTOR_DENOMINATOR 2
#endif

// OBJECTS
// -------

// VECTOR FACET

template <
    typename T,
    typename VoidPtr = void*
>
class vector_facet
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename pointer_traits<VoidPtr>::template rebind<value_type>;
    using const_pointer = typename pointer_traits<VoidPtr>::template rebind<const value_type>;
    using difference_type = typename pointer_traits<VoidPtr>::difference_type;
    using size_type = make_unsigned_t<difference_type>;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // TODO: add iterator, other methods

    // Iterators
    iterator
    begin()
    noexcept
    {
        return iterator(begin_);
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(begin_);
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
        return iterator(end_);
    }

    const_iterator
    end()
    const noexcept
    {
        return const_iterator(end_);
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
        assert(n < size() && "vector[] index out of bounds");
        return begin_[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        assert(n < size() && "vector[] index out of bounds");
        return begin_[n];
    }

    reference
    front()
    {
        assert(!empty() && "front() called for empty vector");
        return *begin_;
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "front() called for empty vector");
        return *begin_;
    }

    reference
    back()
    {
        assert(!empty() && "back() called for empty vector");
        return *(end_ - 1);
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "back() called for empty vector");
        return *(end_ - 1);
    }

    value_type*
    data()
    noexcept
    {
        return to_raw_pointer(begin_);
    }

    const value_type*
    data()
    const noexcept
    {
        return to_raw_pointer(begin_);
    }

    // Capacity
    PYCPP_CPP17_NODISCARD
    bool
    empty()
    const noexcept
    {
        return begin_ == end_;
    }

    size_type
    size()
    const noexcept
    {
        return static_cast<size_type>(end_ - begin_);
    }

    size_type
    max_size()
    const noexcept
    {
        // guaranteed to be constexpr
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    size_type
    capacity()
    const noexcept
    {
        return static_cast<size_type>(end_cap_ - begin_);
    }

    // TODO: add more methods

private:
    pointer begin_;
    pointer end_;
    pointer end_cap_;

    // Modifiers
    void
    swap(
        vector_facet& x
    )
    {
        fast_swap(begin_, x.begin_);
        fast_swap(end_, x.end_);
        fast_swap(end_cap_, x.end_cap_);
    }

    // TODO: private constructors, modifiers, etc...
};

// VECTOR

template <
    typename T,
    typename Allocator = allocator<T>,
    intmax_t GrowthFactorNumerator = PYCPP_VECTOR_GROWTH_FACTOR_NUMERATOR,
    intmax_t GrowthFactorDenominator = PYCPP_VECTOR_GROWTH_FACTOR_DENOMINATOR
>
class vector
{
public:
    using value_type = T;
    using growth_factor = ratio<GrowthFactorNumerator, GrowthFactorDenominator>;
    using allocator_type = Allocator;
    using buffer_type = split_buffer<T, growth_factor::num, growth_factor::den, allocator_type&>;
    using facet_type = vector_facet<
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

    static_assert(
        growth_factor::num > growth_factor::den && growth_factor::num > 0,
        "Growth factor must be a positive ratio."
    );
    // TODO: add here...

    // Constructors
    // TODO

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

    reverse_iterator
    rbegin()
    noexcept
    {
        return facet().rbegin();
    }

    const_reverse_iterator
    rbegin()
    const noexcept
    {
        return facet().rbegin();
    }

    const_reverse_iterator
    crbegin()
    const noexcept
    {
        return facet().crbegin();
    }

    reverse_iterator
    rend()
    noexcept
    {
        return facet().rend();
    }

    const_reverse_iterator
    rend()
    const noexcept
    {
        return facet().rend();
    }

    const_reverse_iterator
    crend()
    const noexcept
    {
        return facet().crend();
    }

    // Capacity
    PYCPP_CPP17_NODISCARD
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
        return facet().size();
    }

    size_type
    max_size()
    const noexcept
    {
        return facet().max_size();
    }

    size_type
    capacity()
    const noexcept
    {
        return facet().capacity();
    }

    // Element access
    reference
    at(
        size_type n
    )
    {
        return facet().at(n);
    }

    const_reference
    at(
        size_type n
    ) const
    {
        return facet().at(n);
    }

    reference
    operator[](
        size_type n
    )
    {
        return facet()[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        return facet()[n];
    }

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

    value_type*
    data()
    noexcept
    {
        return facet().data();
    }

    const value_type*
    data()
    const noexcept
    {
        return facet().data();
    }

    // Modifiers
    void
    clear()
    noexcept
    {
        destruct_at_end(facet().begin_);
    }
    // TODO: insert

    template <typename ... Ts>
    iterator
    emplace(
        const_iterator pos,
        Ts&&... ts
    )
    {
        pointer p = facet().begin_ + (pos - begin());
        if (facet().end_ < facet().end_cap_) {
            if (p == facet().end_) {
                alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), forward<Ts>(ts)...);
                ++facet().end_;
            } else {
                relocate_n(p, facet().end_, p+1);
                ++facet().end_;
                alloc_traits::construct(alloc(), to_raw_pointer(p), forward<Ts>(ts)...);
            }
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + 1), p - facet().begin_, a);
            v.emplace_back(forward<Ts>(ts)...);
            p = swap_out_circular_buffer(v, p);
        }
        return p;
    }

    iterator
    erase(
        const_iterator pos
    )
    {
        assert(pos != end() && "vector::erase(iterator) called with a non-dereferenceable iterator");
        pointer p = facet().begin_ + (pos - begin());
        relocate(p + 1, facet().end_, p);
        destruct_at_end(facet().end_ - 1);
        return p;
    }

    iterator
    erase(
        const_iterator first,
        const_iterator last
    )
    {
        assert(first <= last && "vector::erase(first, last) called with invalid range");
        pointer p = facet().begin_ + (first - begin());
        if (first != last) {
            difference_type ds = last - first;
            relocate(p + ds, facet().end_, p);
            destruct_at_end(facet().end_ - ds);
        }
        return p;
    }

    void
    push_back(
        const_reference x
    )
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), x);
        } else {
            push_back_slow(x);
        }
    }

    void
    push_back(
        value_type&& x
    )
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), move(x));
        } else {
            push_back_slow(move(x));
        }
    }

    template <typename ... Ts>
    reference
    emplace_back(
        Ts&&... ts
    )
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), forward<Ts>(ts)...);
        } else {
            emplace_back_slow(forward<Ts>(ts)...);
        }
        return back();
    }

    void
    pop_back()
    {
        assert(!empty() && "vector::pop_back called for empty vector");
        destruct_at_end(facet().end_ - 1);
    }

    void
    resize(
        size_type sz
    )
    {
        size_type cs = size();
        if (cs < sz) {
            append(sz - cs);
        } else if (cs > sz) {
            destruct_at_end(facet().begin_ + sz);
        }
    }

    void
    resize(
        size_type sz,
        const_reference v
    )
    {
        size_type cs = size();
        if (cs < sz) {
            append(sz - cs, v);
        } else if (cs > sz) {
            destruct_at_end(facet().begin_ + sz);
        }
    }

    void
    swap(
        vector& x
    )
    noexcept
    {
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
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
    using alloc_traits = allocator_traits<allocator_type>;

    compressed_pair<facet_type, allocator_type> data_;

    // Allocator
    allocator_type&
    alloc()
    noexcept
    {
        return get<1>(data_);
    }

    const allocator_type&
    alloc()
    const noexcept
    {
        return get<1>(data_);
    }

    size_type
    recommend(
        size_type new_size
    )
    {
        // get ratio properties
        constexpr intmax_t num = growth_factor::num;
        constexpr intmax_t den = growth_factor::den;
        constexpr double ratio = static_cast<double>(num) / den;

        // check max size
        size_type ms = max_size();
        if (new_size > ms) {
            throw length_error("vector");
        }

        // check with ideal growth rate
        const size_type cap = capacity();
        if (cap >= ms / ratio) {
            return ms;
        }
        return std::max<size_type>(ratio*cap, new_size);
    }

    // TODO: need assign allocs...

    // Object destruction
    void
    destruct_at_end(
        pointer new_last,
        true_type
    )
    noexcept
    {
        facet().end_ = new_last;
    }

    void
    destruct_at_end(
        pointer new_last,
        false_type
    )
    noexcept
    {
        pointer soon_to_be_end = facet().end_;
        while (new_last != soon_to_be_end) {
            alloc_traits::destroy(alloc(), to_raw_pointer(--soon_to_be_end));
        }
        facet().end_ = new_last;
    }

    void
    destruct_at_end(
        pointer new_last
    )
    noexcept
    {
        using bool_type = disjunction<is_trivial<value_type>, is_empty<value_type>>;
        destruct_at_end(new_last, bool_type());
    }

    // Object construction
    void
    construct_at_end(
        size_type n
    )
    {
        allocator_type& a = alloc();
        do {
            alloc_traits::construct(a, to_raw_pointer(facet().end_));
            ++facet().end_;
            --n;
        } while (n > 0);
    }

    void
    construct_at_end(
        size_type n,
        const_reference v
    )
    {
        allocator_type& a = alloc();
        do {
            alloc_traits::construct(a, to_raw_pointer(facet().end_), v);
            ++facet().end_;
            --n;
        } while (n > 0);
    }

    void
    append(
        size_type n
    )
    {
        if (static_cast<size_type>(facet().end_cap_ - facet().end_) >= n) {
            construct_at_end(n);
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + n), size(), a);
            v.construct_at_end(n);
            swap_out_circular_buffer(v);
        }
    }

    void
    append(
        size_type n,
        const_reference v
    )
    {
        if (static_cast<size_type>(facet().end_cap_ - facet().end_) >= n) {
            construct_at_end(n, v);
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + n), size(), a);
            v.construct_at_end(n, v);
            swap_out_circular_buffer(v);
        }
    }

    // Swap out circular buffer
    void
    swap_out_circular_buffer(
        buffer_type& v
    )
    {
        // construct backward calls memmove, if relocatable
        alloc_traits::construct_backward(alloc(), facet().begin_, facet().end_, v.facet().begin_);
        fast_swap(facet().begin_, v.facet().begin_);
        fast_swap(facet().end_, v.facet().end_);
        fast_swap(facet().end_cap_, v.facet().end_cap_);
        v.facet().first_ = v.facet().begin_;
    }

    pointer
    swap_out_circular_buffer(
        buffer_type& v,
        pointer p
    )
    {
        // construct backward calls memmove, if relocatable
        pointer r = v.facet().begin_;
        alloc_traits::construct_backward(alloc(), facet().begin_, p, v.facet().begin_);
        alloc_traits::construct_forward(alloc(), p, facet().end_, v.facet().end_);
        fast_swap(facet().begin_, v.facet().begin_);
        fast_swap(facet().end_, v.facet().end_);
        fast_swap(facet().end_cap_, v.facet().end_cap_);
        v.facet().first_ = v.facet().begin_;
        return r;
    }

    // Push back
    template <typename U>
    void
    push_back_slow(
        U&& x
    )
    {
        allocator_type& a = alloc();
        buffer_type v(recommend(size() + 1), size(), a);
        alloc_traits::construct(a, to_raw_pointer(v.facet().end_), forward<U>(x));
        v.facet().end_++;
        swap_out_circular_buffer(v);
    }

    template <typename ... Ts>
    void
    emplace_back_slow(
        Ts&&... ts
    )
    {
        allocator_type& a = alloc();
        buffer_type v(recommend(size() + 1), size(), a);
        alloc_traits::construct(a, to_raw_pointer(v.facet().end_), forward<Ts>(ts)...);
        v.facet().end_++;
        swap_out_circular_buffer(v);
    }
};

// SPECIALIZATION
// --------------

template <typename T, typename VoidPtr>
struct is_relocatable<vector_facet<T, VoidPtr>>: is_relocatable<VoidPtr>
{};

template <typename T, typename Allocator>
struct is_relocatable<vector<T, Allocator>>:
    bool_constant<
        is_relocatable<vector_facet<T, typename allocator_traits<Allocator>::void_pointer>>::value &&
        is_relocatable<Allocator>::value
    >
{};

PYCPP_END_NAMESPACE
