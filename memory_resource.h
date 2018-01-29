//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <memory_resource> aliases.
 *
 *  For compatibility reasons with C++11 compilers, this is not a strictly
 *  standards conforming implementation of <memory_resource>, however,
 *  it should behave identically for C++11, C++14, and C++17 compilers.
 *
 *  A polymorphic allocator that wraps a virtual, byte-based resource
 *  that abstracts general bytes. The polymorphic allocator is a single
 *  type that may have different allocation properties depending
 *  on the underlying resource.
 *
 *  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3525.pdf
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/memory_resource/polymorphic_allocator.h>

// Right now we depend on some non-standard extensions to
// polymorphic allocator, specifically,
// `propagate_on_container_move_assignment`.
// The conditions to reproduce this are, using
// GCC on Linux:
//      using pair = std::pair<const int, int>;
//      using list = std::list<pair, polymorphic_allocator<pair>>;
//      list x_, y_;
//      y_ = std::move(x_);
//
// This calls `list::operator=(list&&); which then
// checks if `propagate_on_container_move_assignment`
// or `is_always_equal` for the container allocator,
// if not, it uses move assignment to move nodes.
//
// Since the allocator is not `is_always_equal`, since
// it wraps an internal resource, simply manage the
// allocator move assignment so it is **well formed**.
//
// Forcing a custom implementation of polymorphic allocator
// is non-standard behavior, but required right now for C++11
// compilers. Revert this when C++17 becomes well-adopted.
