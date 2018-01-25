//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <cstring> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::size_t;

// Functions
using std::strcpy;
using std::strncpy;
using std::strcat;
using std::strncat;
using std::strxfrm;

// String examination
using std::strlen;
using std::strcmp;
using std::strncmp;
using std::strcoll;
using std::strchr;
using std::strrchr;
using std::strspn;
using std::strcspn;
using std::strpbrk;
using std::strstr;
using std::strtok;

// Character array manipulation
using std::memchr;
using std::memcmp;
using std::memset;
using std::memcpy;
using std::memmove;

// Miscellaneous
using std::strerror;

PYCPP_END_NAMESPACE
