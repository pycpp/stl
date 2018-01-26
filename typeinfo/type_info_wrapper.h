//  :copyright: (c) 2007-2013 Andrey Semashev.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for `std::type_info` to simplify use.
 *
 *  \synopsis
 *      std::string info_to_string(const std::type_info& info);
 *
 *      class type_info_wrapper
 *      {
 *      public:
 *          // Constructors
 *          constexpr type_info_wrapper() noexcept;
 *          constexpr type_info_wrapper(const type_info_wrapper& x) noexcept;
 *          type_info_wrapper(type_info_wrapper&& x) noexcept;
 *          type_info_wrapper(const std::type_info& x) noexcept;
 *
 *          // Assignment
 *          type_info_wrapper& operator=(const type_info_wrapper& x) noexcept;
 *          type_info_wrapper& operator=(type_info_wrapper&& x) noexcept;
 *          type_info_wrapper& operator=(const std::type_info& x) noexcept;
 *
 *          // Observers
 *          explicit operator bool() const noexcept;
 *          bool operator!() const noexcept;
 *          const std::type_info& operator*() const noexcept;
 *          const std::type_info* operator->() const;
 *          bool empty() const noexcept;
 *          const std::type_info& get() const noexcept;
 *          std::string pretty_name() const;
 *
 *          // Modifiers
 *          void swap(type_info_wrapper& x) noexcept;
 *      };
 *
 *      bool operator==(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      bool operator!=(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      bool operator<(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      bool operator>(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      bool operator>=(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      bool operator<=(const type_info_wrapper& x, const type_info_wrapper& y) noexcept;
 *      void swap(type_info_wrapper& x, type_info_wrapper& y) noexcept;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/abi.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/utility/fast_swap.h>
#include <cassert>
#include <string>
#include <typeinfo>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

std::string
info_to_string(
    const std::type_info& info
);

// OBJECTS
// -------

class type_info_wrapper
{
public:
    // Constructors
    constexpr
    type_info_wrapper()
    noexcept:
        info_(nullptr)
    {}

    constexpr
    type_info_wrapper(
        const type_info_wrapper& x
    )
    noexcept:
        info_(x.info_)
    {}

    type_info_wrapper(
        type_info_wrapper&& x
    )
    noexcept:
        info_(x.info_)
    {
        x.info_ = nullptr;
    }

    constexpr
    type_info_wrapper(
        const std::type_info& x
    )
    noexcept:
        info_(&x)
    {}

    // Assignment
    type_info_wrapper&
    operator=(
        const type_info_wrapper& x
    )
    noexcept
    {
        info_ = x.info_;
        return *this;
    }

    type_info_wrapper&
    operator=(
        type_info_wrapper&& x
    )
    noexcept
    {
        info_ = x.info_;
        x.info_ = nullptr;
        return *this;
    }

    type_info_wrapper&
    operator=(
        const std::type_info& x
    )
    noexcept
    {
        info_ = &x;
        return *this;
    }

    // Observers
    explicit
    operator bool()
    const noexcept
    {
        return !empty();
    }

    bool
    operator!()
    const noexcept
    {
        return empty();
    }

    const std::type_info&
    operator*()
    const noexcept
    {
        return get();
    }

    const std::type_info*
    operator->()
    const
    {
        return info_;
    }

    bool
    empty()
    const noexcept
    {
        return info_ == nullptr;
    }

    const std::type_info&
    get()
    const noexcept
    {
        assert(!empty() && "type_info_wrapper must be initialized.");
        return *info_;
    }

    std::string
    pretty_name()
    const
    {
        if (empty()) {
            return "[uninitialized]";
        }

        return info_to_string(get());
    }

    // Modifiers
    void
    swap(
        type_info_wrapper& x
    )
    noexcept
    {
        fast_swap(info_, x.info_);
    }

private:
    const std::type_info* info_;
};

// Relational
inline
bool
operator==(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    auto* xp = x.operator->();
    auto* yp = y.operator->();
    if (xp == yp) {
        return true;
    } else if (xp && yp) {
        return *xp == *yp;
    }
    return false;
}

inline
bool
operator!=(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    return !(x == y);
}

inline
bool
operator<(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    auto* xp = x.operator->();
    auto* yp = y.operator->();
    if (xp && yp) {
        return static_cast<bool>(x->before(*y));
    }
    // use pointer arithmetic then
    return xp < yp;
}

inline
bool
operator>(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    return y < x;
}

inline
bool
operator>=(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    return !(x < y);
}

inline
bool
operator<=(
    const type_info_wrapper& x,
    const type_info_wrapper& y
)
noexcept
{
    return !(y < x);
}

inline
void
swap(
    type_info_wrapper& x,
    type_info_wrapper& y
)
noexcept
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
