//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <regex> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <regex>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

namespace regex_constants
{

using std::regex_constants::syntax_option_type;
using std::regex_constants::match_flag_type;
using std::regex_constants::error_type;

}   /* regex_constants */

// Classes
using std::basic_regex;
using std::sub_match;
using std::match_results;
using std::regex_iterator;
using std::regex_token_iterator;
using std::regex_error;
using std::regex_traits;

// Algorithm
using std::regex_match;
using std::regex_search;
using std::regex_replace;

// Non-member operations
using std::swap;

PYCPP_END_NAMESPACE
