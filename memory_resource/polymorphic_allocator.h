//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Polymorphic allocator using a virtual resource.
 *
 *  \synopsis
 *      template <typename T>
 *      struct polymorphic_allocator
 *      {
 *          using value_type = T;
 *          polymorphic_allocator() noexcept;
 *          polymorphic_allocator(memory_resource *r);
 *          polymorphic_allocator(const polymorphic_allocator<T>& rhs);
 *          template <typename U> polymorphic_allocator(const polymorphic_allocator<U>& rhs) noexcept;
 *          polymorphic_allocator<T>& operator=(const polymorphic_allocator<T>& rhs) = delete;
 *
 *          T* allocate(size_t n);
 *          void deallocate(T* p, size_t n);
 *          polymorphic_allocator select_on_container_copy_construction() const;
 *          memory_resource* resource() const;
 *      };
 *
 *      memory_resource* new_delete_resource() noexcept;
 *      memory_resource* null_memory_resource() noexcept;
 *      memory_resource* get_default_resource() noexcept;
 *      memory_resource* set_default_resource(memory_resource* r) noexcept;
 */

#include <pycpp/stl/memory_resource/memory_resource.h>
#include <pycpp/stl/memory_resource/new_delete_resource.h>
#include <pycpp/stl/memory_resource/null_memory_resource.h>
#include <pycpp/stl/memory_resource/resource_adaptor.h>

PYCPP_BEGIN_NAMESPACE

namespace pmr
{
// FUNCTIONS
// ---------

/**
 *  \brief Return a pointer to a global resource that uses `new` and `delete`.
 */
inline
memory_resource*
new_delete_resource()
noexcept
{
    static struct new_delete_resource singleton;
    return &singleton;
}

/**
 *  \brief Return a pointer to a global resource that does no allocation.
 */
inline
memory_resource*
null_memory_resource()
noexcept
{
    static struct null_memory_resource singleton;
    return &singleton;
}

/**
 *  \brief Get the current default resource.
 */
inline
memory_resource*
get_default_resource()
noexcept
{
    memory_resource* r = memory_resource::default_resource_.load();
    if (r == nullptr) {
        r = new_delete_resource();
    }

    return r;
}

/**
 *  \brief Set the default resource.
 */
inline
memory_resource*
set_default_resource(
    memory_resource* r
)
noexcept
{
    if (r == nullptr) {
        r = new_delete_resource();
    }

    return memory_resource::default_resource_.exchange(r);
}


// OBJECTS
// -------

/**
 *  \brief STL allocator wrapping the polymorphic resource.
 */
template <typename T>
class polymorphic_allocator
{
public:
    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;

    // Constructors
    polymorphic_allocator(const polymorphic_allocator<T>&) = default;
    polymorphic_allocator<T>& operator=(const polymorphic_allocator<T>&) = delete;

    polymorphic_allocator()
    noexcept:
        resource_(get_default_resource())
    {}

    polymorphic_allocator(
        memory_resource *r
    ):
        resource_(r ? r : get_default_resource())
    {}

    template <typename U>
    polymorphic_allocator(
        const polymorphic_allocator<U>& rhs
    )
    noexcept:
        resource_(rhs.resource())
    {}

    polymorphic_allocator(
        polymorphic_allocator<T>&& rhs
    )
    noexcept:
        polymorphic_allocator()
    {
        // let resource use the default constructor
        std::swap(resource_, rhs.resource_);
    }

    template <typename U>
    polymorphic_allocator(
        polymorphic_allocator<U>&& rhs
    )
    noexcept:
        polymorphic_allocator()
    {
        // let resource use the default constructor
        std::swap(resource_, rhs.resource_);
    }

    polymorphic_allocator<T>&
    operator=(polymorphic_allocator<T>&& rhs) noexcept
    {
        std::swap(resource_, rhs.resource_);
        return *this;
    }

    // Allocator traits
    T*
    allocate(
        size_t n
    )
    {
        return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T)));
    }

    T*
    reallocate(
        T* p,
        size_t old_size,
        size_t new_size,
        size_t n,
        size_t old_offset,
        size_t new_offset
    )
    {
        size_t old_bytes = old_size * sizeof(T);
        size_t new_bytes = new_size * sizeof(T);
        size_t count = n * sizeof(T);
        size_t old_off = old_offset * sizeof(T);
        size_t new_off = new_offset * sizeof(T);
        return static_cast<T*>(resource_->reallocate(p, old_bytes, new_bytes, count, old_off, new_off, alignof(T)));
    }

    void
    deallocate(
        T* p,
        size_t n
    )
    {
        resource_->deallocate(p, n * sizeof(T), alignof(T));
    }

    // Properties
    polymorphic_allocator
    select_on_container_copy_construction()
    const
    {
        return polymorphic_allocator<T>(resource_);
    }

    memory_resource*
    resource()
    const
    {
        return resource_;
    }

private:
    memory_resource* resource_;

    template <typename U>
    friend class polymorphic_allocator;
};

template <typename T1, typename T2>
inline
bool
operator==(
    const polymorphic_allocator<T1>& x,
    const polymorphic_allocator<T2>& y
)
{
    return *x.resource() == *y.resource();
}

template <typename T1, typename T2>
inline
bool
operator!=(
    const polymorphic_allocator<T1>& x,
    const polymorphic_allocator<T2>& y
)
{
    return !(x == y);
}

}   /* pmr */

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable;

template <>
struct is_relocatable<pmr::memory_resource>: std::true_type
{};

template <typename Allocator>
struct is_relocatable<pmr::resource_adaptor_impl<Allocator>>: is_relocatable<Allocator>
{};

template <typename T>
struct is_relocatable<pmr::polymorphic_allocator<T>>: std::true_type
{};

PYCPP_END_NAMESPACE
