//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/memory_resource/memory_resource.h>

PYCPP_BEGIN_NAMESPACE

namespace pmr
{
// OBJECTS
// -------

std::atomic<memory_resource*>
memory_resource::default_resource_ = ATOMIC_VAR_INIT(nullptr);

}   /* pmr */

PYCPP_END_NAMESPACE
