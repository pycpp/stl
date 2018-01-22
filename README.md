# STL

**Table of Contents**

- [Introduction](#introduction)

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

**Type Erase**

The type-erased containers are called "facets", to avoid any conflicts with `basic_string_view`. Although a complete erasure of the allocator is impossible from the facet, due to the possibility of using [shared memory to allocate items])(http://blog.nuggetwheat.org/index.php/2015/09/01/why-pointer_traits-was-introduced-in-c11/), passing a `void_pointer` is sufficient for facet operations. In most cases, `void_pointer` will be equivalent to `void*` regardless of the allocator, allowing both the facet and the container's iterator to be used interchangeably regardless of the allocator.

// TODO: continue`


## Progress

// TODO: remove this section

Finished:
    type_traits.h
        - Unittests
    memory.h
        - Unittests

Working on:
    - typeinfo
