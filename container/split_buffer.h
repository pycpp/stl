//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Split buffer type to facilitate container implementations.
 *
 *  Highly optimized buffer with leading and trailing uninitialized
 *  values, making it useful for deque and vector operations.
 *
 *  \synopsis
 // TODO: implement
 */

#pragma once

#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/cassert.h>
#include <pycpp/stl/cmath.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/ratio.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/container/compressed_pair.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// Buffer growth factor of 2.0 is the worst possible.
//      https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md#memory-handling
#ifndef PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_NUMERATOR
#   define PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_NUMERATOR 3
#endif

#ifndef PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_DENOMINATOR
#   define PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_DENOMINATOR 2
#endif

// OBJECTS
// -------

// SPLICE BUFFER FACET

template <typename T, typename VoidPtr = void*>
class split_buffer_facet
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

    // Constructors
    split_buffer_facet()
    noexcept:
        first_(nullptr),
        begin_(nullptr),
        end_(nullptr),
        end_cap_(nullptr)
    {}

    // Iterators
    iterator
    begin()
    noexcept
    {
        return begin_;
    }

    const_iterator
    begin()
    const noexcept
    {
        return begin_;
    }

    iterator
    end()
    noexcept
    {
        return end_;
    }

    const_iterator
    end()
    const noexcept
    {
        return end_;
    }

    // Capacity
    size_type
    size()
    const
    {
        return static_cast<size_type>(end_ - begin_);
    }

    bool
    empty()
    const
    {
        return end_ == begin_;
    }

    size_type
    capacity()
    const
    {
        return static_cast<size_type>(end_cap_ - first_);
    }

    size_type
    front_spare()
    const
    {
        return static_cast<size_type>(begin_ - first_);
    }

    size_type
    has_front_spare()
    const
    {
        return first_ != begin_;
    }

    size_type
    back_spare()
    const
    {
        return static_cast<size_type>(end_cap_ - end_);
    }

    size_type
    has_back_spare()
    const
    {
        return end_ != end_cap_;
    }

    // Element access
    reference
    front()
    {
        return *begin_;
    }

    const_reference
    front()
    const
    {
        return *begin_;
    }

    reference
    back()
    {
        return *(end_ - 1);
    }

    const_reference
    back()
    const
    {
        return *(end_ - 1);
    }

//private:
    // Implied private from here on
    pointer first_;
    pointer begin_;
    pointer end_;
    pointer end_cap_;

    // Constructors
    // Only allow uninitialized constructing privately.
    split_buffer_facet(nullptr_t)
    noexcept
    {}

    split_buffer_facet(const split_buffer_facet&) = delete;

    split_buffer_facet(
        split_buffer_facet&& x
    )
    noexcept:
        first_(move(x.first_)),
        begin_(move(x.begin_)),
        end_(move(x.end_)),
        end_cap_(move(x.end_cap_))
    {
        x.first_ = x.begin_ = x.end_ = x.end_cap_ = nullptr;
    }

    // Assignment
    split_buffer_facet& operator=(const split_buffer_facet&) = delete;

    split_buffer_facet&
    operator=(
        split_buffer_facet&& x
    )
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void swap(
        split_buffer_facet& x
    )
    noexcept
    {
        fast_swap(first_, x.first_);
        fast_swap(begin_, x.begin_);
        fast_swap(end_, x.end_);
        fast_swap(end_cap_, x.end_cap_);
    }
};


// SPLIT BUFFER

template <
    typename T,
    std::intmax_t GrowthFactorNumerator = PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_NUMERATOR,
    std::intmax_t GrowthFactorDenominator = PYCPP_SPLIT_BUFFER_GROWTH_FACTOR_DENOMINATOR,
    typename Allocator = allocator<T>
>
class split_buffer
{
public:
    using value_type = T;
    using growth_factor = std::ratio<GrowthFactorNumerator, GrowthFactorDenominator>;
    using allocator_type = Allocator;
    using alloc_rr = remove_reference_t<allocator_type>;
    using alloc_traits = allocator_traits<alloc_rr>;
    using facet_type = split_buffer_facet<T, typename alloc_traits::void_pointer>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using size_type = typename alloc_traits::size_type;
    using difference_type = typename alloc_traits::difference_type;
    using iterator = pointer;
    using const_iterator = const_pointer;

    static_assert(
        growth_factor::num > growth_factor::den && growth_factor::num > 0,
        "Growth factor must be a positive ratio."
    );

    compressed_pair<facet_type, allocator_type> data_;

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

    // Alloc
    alloc_rr&
    alloc()
    noexcept
    {
        return get<1>(data_);
    }

    const alloc_rr&
    alloc()
    const noexcept
    {
        return get<1>(data_);
    }

    void
    move_assign_alloc(
        split_buffer& x
    )
    noexcept
    {
        using propogate = typename alloc_traits::propagate_on_container_move_assignment;
        move_assign_alloc(x, integral_constant<bool, propogate::value>());
    }

    void
    move_assign_alloc(
        split_buffer& x,
        true_type
    )
    noexcept
    {
        alloc() = move(x.alloc());
    }

    void
    move_assign_alloc(
        split_buffer&,
        false_type
    )
    noexcept
    {}

    // Constructors
    // This constructor will fail if using a reference allocator.
    split_buffer()
    noexcept
    {}

    split_buffer(
        size_type cap,
        size_type start,
        alloc_rr& a
    ):
        data_(facet_type(nullptr), a)
    {
        facet().first_ = cap != 0 ? alloc_traits::allocate(alloc(), cap) : nullptr;
        facet().begin_ = facet().end_ = facet().first_ + start;
        facet().end_cap_ = facet().first_ + cap;
    }

    split_buffer(const alloc_rr& a)
    noexcept:
        data_(a)
    {}

    split_buffer(
        split_buffer&& x
    )
    noexcept:
        data_(move(x.facet()), move(x.alloc()))
    {}

    split_buffer(
        split_buffer&& x,
        const alloc_rr& a
    ):
        data_(a)
    {
        if (a == x.alloc()) {
            facet().first_ = x.facet().first_;
            facet().begin_ = x.facet().begin_;
            facet().end_ = x.facet().end_;
            facet().end_cap_ = x.facet().end_cap_;
            x.facet().first_ = x.facet().begin_ = x.facet().end_ = x.facet().end_cap_ = nullptr;
        } else {
            size_type cap = x.size();
            facet().first_ = alloc_traits::allocate(alloc(), cap);
            relocate(x.begin(), x.end(), facet().first_);
            facet().begin_ = facet().first_;
            facet().end_ = facet().begin_ + cap;
            facet().end_cap_ = facet().first_ + cap;
        }
    }

    // Destructors
    ~split_buffer()
    {
        clear();
        if (facet().first_) {
            alloc_traits::deallocate(alloc(), facet().first_, capacity());
        }
    }

    // Assignment
    split_buffer&
    operator=(
        split_buffer&& x
    )
    {
        clear();
        shrink_to_fit();
        facet().first_ = x.facet().first_;
        facet().begin_ = x.facet().begin_;
        facet().end_ = x.facet().end_;
        facet().end_cap_ = x.facet().end_cap_;
        move_assign_alloc(x);
        x.facet().first_ = x.facet().begin_ = x.facet().end_ = x.facet().end_cap_ = nullptr;

        return *this;
    }

    split_buffer(const split_buffer&) = delete;
    split_buffer& operator=(const split_buffer&) = delete;

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

    // Capacity
    size_type
    size()
    const
    {
        return facet().size();
    }

    bool
    empty()
    const
    {
        return facet().empty();
    }

    size_type
    capacity()
    const
    {
        return facet().capacity();
    }

    size_type
    front_spare()
    const
    {
        return facet().front_spare();
    }

    bool
    has_front_spare()
    const
    {
        return facet().has_front_spare();
    }

    size_type
    back_spare()
    const
    {
        return facet().back_spare();
    }

    bool
    has_back_spare()
    const
    {
        return facet().has_back_spare();
    }

    void
    reserve(
        size_type n
    )
    {
        if (n < capacity()) {
            internal_resize(n, 0);
        }
    }

    void
    shrink_to_fit()
    noexcept
    {
        if (capacity() > size()) {
            try {
                internal_resize(size(), 0);
            } catch (...) {
            }
        }
    }

    // Internal resize, does not default initialize any values.
    void
    internal_resize(
        size_type new_size,
        size_type new_offset
    )
    {
        // get parameters
        assert(new_size > new_offset && "Buffer overflow.");
        size_type old_offset = front_spare();
        size_type sz = size();
        size_type count = std::min(new_size-new_offset, sz);

        // reallocate and calculate new buffer positions
        pointer new_first = alloc_traits::reallocate(alloc(), facet().first_, capacity(), new_size, count, old_offset, new_offset);
        pointer new_begin = new_first + new_offset;
        pointer new_end = new_begin + sz;
        pointer new_cap = new_first + new_size;

        // swap into new buffer
        fast_swap(facet().first_, new_first);
        fast_swap(facet().begin_, new_begin);
        fast_swap(facet().end_, new_end);
        fast_swap(facet().end_cap_, new_cap);
    }

    // Guess default size and offset
    template <bool AlignBack = false>
    void
    internal_resize()
    {
        // get the ratio for our growth
        constexpr std::intmax_t num = growth_factor::num;
        constexpr std::intmax_t den = growth_factor::den;
        constexpr double ratio = static_cast<double>(num) / den;
        const std::intmax_t inum = AlignBack ? 3 : 0;

        // get reallocation size, max(ratio * capacity(), 1);
        // internally resize and align data from 25%-75% of new buffer
        size_type c = std::max<size_type>(ceil(ratio * capacity()), 1);
        internal_resize(c, (c + inum) / 4);
    }

    void
    internal_move_back()
    {
        // move halfway (incrementing up) to the back
        difference_type d = facet().end_cap_ - facet().end_;
        d = (d + 1) / 2;

        // relocate in the overlapping range, and reset views
        relocate(facet().begin_, facet().end_, facet().begin_+d);
        facet().begin_ += d;
        facet().end_ += d;
    }

    void
    internal_move_front()
    {
        // Moving forward...
        difference_type d = facet().begin_ - facet().first_;
        d = (d + 1) / 2;

        // relocate in the overlapping range, and reset views
        relocate(facet().begin_, facet().end_, facet().begin_-d);
        facet().begin_ -= d;
        facet().end_ -= d;
    }

    void
    shift_back()
    {
        if (has_back_spare()) {
            internal_move_back();
        } else {
            internal_resize<true>();
        }
    }

    void
    check_shift_back()
    {
        if (!has_front_spare()) {
            shift_back();
        }
    }

    void
    shift_front()
    {
        if (has_front_spare()) {
            internal_move_front();
        } else {
            internal_resize<false>();
        }
    }

    void
    check_shift_front()
    {
        if (!has_back_spare()) {
            shift_front();
        }
    }

    // Modifiers
    void
    clear()
    noexcept
    {
        destruct_at_end(facet().begin_);
    }

    void
    pop_front()
    {
        destruct_at_begin(facet().begin_+1);
    }

    void
    pop_back()
    {
        destruct_at_end(facet().end_-1);
    }

    void
    swap(
        split_buffer& x
    )
    noexcept
    {
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
    }

    void
    push_front(
        const_reference v
    )
    {
        check_shift_back();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().begin_-1), v);
        --facet().begin_;
    }

    void
    push_front(
        value_type&& v
    )
    {
        check_shift_back();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().begin_-1), std::move(v));
        --facet().begin_;
    }

    template <typename ... Ts>
    void
    emplace_front(
        Ts&&... ts
    )
    {
        check_shift_back();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().begin_-1), std::forward<Ts>(ts)...);
        --facet().begin_;
    }

    void
    push_back(
        const_reference v
    )
    {
        check_shift_front();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), v);
        ++facet().end_;
    }

    void
    push_back(
        value_type&& v
    )
    {
        check_shift_front();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), std::move(v));
        ++facet().end_;
    }

    template <typename ... Ts>
    void
    emplace_back(
        Ts&&... ts
    )
    {
        check_shift_front();
        alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), std::forward<Ts>(ts)...);
        ++facet().end_;
    }

    // Construct at
    void
    construct_at_end(
        size_type n
    )
    {
        alloc_rr& a = alloc();
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
        alloc_rr& a = alloc();
        do {
            alloc_traits::construct(a, to_raw_pointer(facet().end_), v);
            ++facet().end_;
            --n;
        } while (n > 0);
    }

    // Destruct at
    void destruct_at_begin(
        pointer new_begin
    )
    {
        destruct_at_begin(new_begin, is_trivial<value_type>());
    }

    void
    destruct_at_begin(
        pointer new_begin,
        false_type
    )
    {
        while (facet().begin_ != new_begin) {
            alloc_traits::destroy(alloc(), to_raw_pointer(facet().begin_++));
        }
    }

    void
    destruct_at_begin(
        pointer new_begin,
        true_type
    )
    {
        facet().begin_ = new_begin;
    }

    void
    destruct_at_end(
        pointer new_last
    )
    noexcept
    {
        destruct_at_end(new_last, is_trivial<value_type>());
    }

    void
    destruct_at_end(
        pointer new_last,
        false_type
    )
    noexcept
    {
        while (facet().end_ != new_last) {
            alloc_traits::destroy(alloc(), to_raw_pointer(facet().end_--));
        }
    }

    void
    destruct_at_end(
        pointer new_last,
        true_type
    )
    noexcept
    {
        facet().end_ = new_last;
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
};

template <typename T, std::intmax_t N, std::intmax_t D, typename Allocator>
inline
void
swap(
    split_buffer<T, N, D, Allocator>& x,
    split_buffer<T, N, D, Allocator>& y
)
noexcept
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
