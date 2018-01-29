//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <iosfwd> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <iosfwd>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// string
template <typename Char>
class char_traits;
// TODO: inherit char traits for forward declarations
//using std::char_traits;

// memory
using std::allocator;

// ios
using std::basic_ios;
using std::fpos;

// streambuf
using std::basic_streambuf;

// ostream
using std::basic_ostream;

// istream
using std::basic_istream;
using std::basic_iostream;

// typedefs
using std::streampos;
using std::wstreampos;

// TODO: need to define custom fstream shit
// TODO: need to define custom sstream shit
// TODO: need to define custom syncstream shit

PYCPP_END_NAMESPACE
