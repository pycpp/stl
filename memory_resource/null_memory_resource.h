//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Memory resource that does not allocate memory.
 *
 *  \synopsis
*       struct null_memory_resource: public memory_resource
*       {
*       public:
*           ~null_memory_resource() = default;
*
*       protected:
*           virtual void* do_allocate(size_t, size_t) override;
*           virtual void do_deallocate(void*, size_t, size_t) override;
*       };
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

struct null_memory_resource: public memory_resource
{
public:
    ~null_memory_resource() = default;

protected:
    virtual
    void*
    do_allocate(size_t, size_t)
    override
    {
        throw std::bad_alloc();
    }

    virtual
    void
    do_deallocate(void* p, size_t n, size_t)
    override
    {
        assert(p == nullptr || n == 0);
    }
};

}   /* pmr */

PYCPP_END_NAMESPACE
