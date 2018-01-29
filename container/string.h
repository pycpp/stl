//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief STL string with allocator erasure from iterators.
 *
 *  \synopsis
 // TODO: implement
 */

// TODO: implement using the relocatable vector...

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename Char>
class char_traits: public std::char_traits<Char>
{};

template <typename Char, typename Traits, typename Allocator>
class basic_string: public std::basic_string<Char, Traits, Allocator>
{
    using std::basic_string<Char, Traits, Allocator>::basic_string;
};

// TODO: actually implement them...

PYCPP_END_NAMESPACE
