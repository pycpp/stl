//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <iterator> aliases.
 */

#pragma once

#include <pycpp/stl/iterator/cbegin.h>
#include <pycpp/stl/iterator/cend.h>
#include <pycpp/stl/iterator/make_reverse_iterator.h>
#include <pycpp/stl/iterator/rbegin.h>
#include <pycpp/stl/iterator/rend.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Primitives
using std::iterator_traits;
using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;
using std::iterator;

// Adaptors
using std::reverse_iterator;
using std::move_iterator;
using std::back_insert_iterator;
using std::front_insert_iterator;
using std::insert_iterator;

// Stream Iterators
using std::istream_iterator;
using std::ostream_iterator;
using std::istreambuf_iterator;
using std::ostreambuf_iterator;

// Functions
using std::make_move_iterator;
using std::front_inserter;
using std::back_inserter;
using std::inserter;
using std::advance;
using std::distance;
using std::next;
using std::prev;
using std::begin;
using std::end;

PYCPP_END_NAMESPACE
