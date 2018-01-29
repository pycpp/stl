//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <cwchar> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cwchar>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::mbstate_t;
using std::size_t;
using std::wint_t;

// String manipulation
using std::wcscpy;
using std::wcsncpy;
using std::wcscat;
using std::wcsncat;
using std::wcsxfrm;

// String examination
using std::wcslen;
using std::wcscmp;
using std::wcsncmp;
using std::wcscoll;
using std::wcschr;
using std::wcsrchr;
using std::wcsspn;
using std::wcscspn;
using std::wcspbrk;
using std::wcsstr;
using std::wcstok;

// Wide character array manipulation
using std::wmemcpy;
using std::wmemmove;
using std::wmemcmp;
using std::wmemchr;
using std::wmemset;

// Multibyte/wide character conversion
using std::mbsinit;
using std::btowc;
using std::wctob;
using std::mbrlen;
using std::mbrtowc;
using std::wcrtomb;
using std::mbsrtowcs;
using std::wcsrtombs;

// Input/Output
using std::fgetwc;
using std::getwc;
using std::fgetws;
using std::fputwc;
using std::putwc;
using std::fputws;
using std::getwchar;
using std::putwchar;
using std::ungetwc;
using std::fwide;
using std::wscanf;
using std::fwscanf;
using std::swscanf;
using std::vwscanf;
using std::vfwscanf;
using std::vswscanf;
using std::wprintf;
using std::fwprintf;
using std::swprintf;
using std::vwprintf;
using std::vfwprintf;
using std::vswprintf;

// String conversions
using std::wcsftime;
using std::wcstol;
using std::wcstoll;
using std::wcstoul;
using std::wcstoull;
using std::wcstof;
using std::wcstod;
using std::wcstold;

PYCPP_END_NAMESPACE
