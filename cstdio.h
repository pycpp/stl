//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <cstdio> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <cstdio>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Types
using std::FILE;
using std::fpos_t;
using std::size_t;

// File access
using std::fopen;
using std::freopen;
using std::fclose;
using std::fflush;
using std::setbuf;
using std::setvbuf;

// Direct input/output
using std::fread;
using std::fwrite;

// Narrow character
using std::fgetc;
using std::getc;
using std::fgets;
using std::fputc;
using std::putc;
using std::fputs;
using std::getchar;
using std::putchar;
using std::puts;
using std::ungetc;

// Narrow/multibyte character
using std::scanf;
using std::fscanf;
using std::sscanf;
using std::vscanf;
using std::vfscanf;
using std::vsscanf;
using std::printf;
using std::fprintf;
using std::sprintf;
using std::snprintf;
using std::vprintf;
using std::vfprintf;
using std::vsprintf;
using std::vsnprintf;

// File positioning
using std::ftell;
using std::fgetpos;
using std::fseek;
using std::fsetpos;
using std::rewind;

// Error handling
using std::clearerr;
using std::feof;
using std::ferror;
using std::perror;

// Operations on files
using std::remove;
using std::rename;
using std::tmpfile;
using std::tmpnam;

PYCPP_END_NAMESPACE
