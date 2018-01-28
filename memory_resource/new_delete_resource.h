//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Memory resource that uses new/delete.
 *
 *  \synopsis
 *      struct new_delete_resource: public memory_resource
 *      {
 *      public:
 *          ~new_delete_resource() = default;
 *
 *      protected:
 *          virtual void* do_allocate(size_t, size_t) override;
 *          virtual void do_deallocate(void*, size_t, size_t) override;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/memory_resource/memory_resource.h>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

namespace pmr
{
// OBJECTS
// -------

struct new_delete_resource: public memory_resource
{
public:
    ~new_delete_resource() = default;

protected:
    virtual
    void*
    do_allocate(
        size_t n,
        size_t alignment
    )
    override
    {
        return operator new(n);
    }

    virtual
    void
    do_deallocate(
        void* p,
        size_t n,
        size_t alignment
    )
    override
    {
        operator delete(p);
    }
};

}   /* pmr */

PYCPP_END_NAMESPACE
