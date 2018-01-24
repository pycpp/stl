# STL

**Table of Contents**

- [Introduction](#introduction)
- [Containers](#containers)
- [Functional](#functional)
- [Memory](#memory)
- [Thread](#thread)
- [Type Traits](#type-traits)
  - [Type Trait Extensions](#type-trait-extensions)

## Introduction

The standard library module defines an STL-like interface with numerous corrections for perceived flaws of the C++ standard library.

## Containers

PyCPP attempts to fully separate memory allocation from container data, to erase the allocator type from the largest subset of operations possible.

**Problem**  

Consider I have an interface, which takes a `std::map<int, int>`, processes this data, and returns some value of success or failure. Only, this method is prohibitively large, or due to intellectual property reasons, I do not wish to make it a public header.

```cpp
class Processor
{
    void call(const std::map<int, int>& m);
};
```

Now, let us consider that creating the `std::map` takes prohibitively long, due to the large numbers of items required. To speed things up, I wish to use a custom allocator. However, changing the map's allocator changes the map's type signature, making the new map incompatible with the existing interface. Therefore, despite no requirements for modifications to the map's memory in our method, we are unable to access it's data without knowing the map's allocator.

**Solution**  

// TODO

By defining a subset of container methods, including the iterator interface, without requiring the allocator type, we can cast our class to a minimal 

**Allocator Erasure**

The type-erased containers are called "facets", to avoid any conflicts with `basic_string_view`. Although a complete erasure of the allocator is impossible from the facet, due to the possibility of using [shared memory to allocate items])(http://blog.nuggetwheat.org/index.php/2015/09/01/why-pointer_traits-was-introduced-in-c11/), passing a `void_pointer` is sufficient for facet operations. In most cases, `void_pointer` will be equivalent to `void*` regardless of the allocator, allowing both the facet and the container's iterator to be used interchangeably regardless of the allocator.

// TODO: continue`

## Functional

// TODO: document
// TODO: mention how slow std::hash can be...

## Memory

// TODO: document

## Thread

// TODO: document

## Type Traits

PyCPP contains a complete backport of C++17 (and proposed C++20) type traits to C++11, in addition to various extensions.

### Type Trait Extensions

**Has Reallocate**

Detect if an allocator has a specialized rellocation method with the signature `pointer reallocate(pointer p, size_t old_size, size_t new_size)`, allowing fast buffer reallocating with movable types.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

template <typename T>
struct cstdlib_allocator
{
    using value_type = T;
    using pointer = value_type*;
    using size_type = unsigned;

    pointer reallocate(pointer, size_type, size_type);
};

static_assert(!has_reallocate<std::allocator<char>>::value, "");
static_assert(!has_reallocate<cstdlib_allocator<char>>::value, "");
```

**Identity**

Wrap type so `identity<T>::type` is the same as `T`.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

static_assert(is_same<typename identity<int>::type, int>::value, "");
static_assert(is_same<identity_t<int>, int>::value, "");
```

**Is Complete***

Detect if type is a complete type.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

struct incomplete;

static_assert(is_complete<int>::value, "");
static_assert(!is_complete<incomplete>::value, "");
```

**Is Iterator**

Detect iterator category or iterator concept at compile-time. Two classes of type traits exist: `is_*_iterator` detects if there is an exact iterator category match, while `is_*_iterable` detects if the iterator supports all methods required by that iterator category. For example, `is_input_iterator` is false for random-access iterators, however, `is_input_iterable` is true for random-access iterators.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

static_assert(!is_input_iterator<int*>::value, "");
static_assert(is_random_access_iterator<int*>::value, "");
static_assert(is_input_iterable<int*>::value, "");
static_assert(is_random_access_iterable<int*>::value, "");
```

**Is Reference Wrapper**

Detect if type is a specialization of `reference_wrapper`.

```cpp
#include <pycpp/stl/functional.h>
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

struct incomplete;

static_assert(!is_reference_wrapper<int>::value, "");
static_assert(is_reference_wrapper<reference_wrapper<int>>::value, "");
```

**Is Relocatable**

Detect if a type is relocatable (can be relocated via `std::memcpy`) at compile-time. By default, only trivially copyable or trivially movable types are relocatable, however, `is_relocatable` may be specialized to mark complex types as relocatable. Nearly every C++ type is relocatable, except for those with virtual methods or pointers to internal variables. Specializing `is_relocatable` for a type may enable significant optimizations for PyCPP containers.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

struct pod_struct
{
    int x;
    int y;
};

struct virtual_struct
{
    virtual ~virtual_struct();
};

static_assert(is_relocatable<pod_struct>::value, "");
static_assert(!is_relocatable<virtual_struct>::value, "");
```

**Is Safe Overload**

Check if the first item in a variadic parameter pack is not the same as a given type, optionally removing references prior to type checking. Useful for overloading constructors using `enable_safe_overload_t`, allowing both a copy constructor and a constructor accepting a variadic parameter pack.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

static_assert(!is_safe_overload<false, int, int>::value, "");
static_assert(is_safe_overload<false, int, int&>::value, "");
static_assert(!is_safe_overload<true, int, int&>::value, "");
static_assert(is_safe_overload<true, int, float>::value, "");
```

**Is Swappable**

In addition to C++17 backports of `is_swappable_with`, `is_nothrow_swappable_with`,  `is_swappable`, and `is_nothrow_swappable`, PyCPP also includes `enable_*`, which evaluate to `void` if the types are swappable, and lead to substitution failure otherwise.

**Is Trivial**

Due to the partial support of `is_trivially_*` type traits in early C++11 compilers, we include backports for all support compilers, and include `enable_*` traits, which enable substitution failure if the type does not support a trivial operation.

**Logical***

In addition to C++17 backports of `conjuction`, `disjunction`, and `negation`, PyCPP also includes various helpers to simplify template metaprogramming. `type_map_and`, `type_map_or`, and `type_not` are analogous to `conjuction`, `disjunction`, and `negation`, respectively, but take a `template <typename>` as their first argument, facilitating the wrapping of generic types.

```cpp
#include <pycpp/stl/type_traits.h>

PYCPP_USING_NAMESPACE

static_assert(type_map_and<identity_t, true_type, true_type>::value, "");
static_assert(type_map_or<identity_t, true_type, false_type>::value, "");
static_assert(type_not<identity_t, false_type>::value, "");
```

## Progress

// TODO: remove this section

Finished:
    type_traits.h
        - Unittests
    memory.h
        - Unittests
        - shared_ptr
        - construct_forward
            - unittests
        - construct_range_forward
            - unittests
        - construct_backward
            - unittests
        - checked_delete
            - unittests
    - forward_list.cc
        - Unittests

Working on:
    - typeinfo

