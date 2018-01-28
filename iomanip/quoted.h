//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief `quoted` backport for C++11.
 *
 *  \synopsis
 *      // TODO: implement...
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/ios/save_flags.h>
#include <iomanip>
#include <istream>
#include <ostream>
#include <string>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(PYCPP_CPP14)            // CPP14

using std::quoted;

#else                               // <=CPP11

// HELPERS
// -------

template <typename Char, typename Traits, typename ForwardIterator>
static
std::basic_ostream<Char, Traits>&
quoted_output(
    std::basic_ostream<Char, Traits>& os,
    ForwardIterator first, ForwardIterator last,
    Char delim,
    Char escape
)
{
    std::basic_string<Char, Traits> str;
    str.push_back(delim);
    for ( ; first != last; ++ first) {
        if (Traits::eq(*first, escape) || Traits::eq(*first, delim)) {
            str.push_back(escape);
        }
        str.push_back(*first);
    }
    str.push_back(delim);
    return os << str;
}

template <typename Char, typename Traits, typename String>
static
std::basic_istream<Char, Traits>&
quoted_input(
    std::basic_istream<Char, Traits>& is,
    String& string,
    Char delim,
    Char escape
)
{
    string.clear();
    Char c;
    is >> c;
    if (is.fail()) {
        return is;
    }

    if (!Traits::eq(c, delim)) {
        is.unget();
        is >> string;
        return is;
    }

    save_flags<Char, Traits> sf(is);
    std::noskipws(is);
    while (true) {
        is >> c;
        if (is.fail()) {
            break;
        }
        if (Traits::eq(c, escape)) {
            is >> c;
            if (is.fail()) {
                break;
            }
        } else if (Traits::eq (c, delim)) {
            break;
        }
        string.push_back ( c );
    }

    return is;
}


//template <typename Char, typename Traits, typename _Iter>
//basic_ostream<Char, Traits>& operator<<(
//         basic_ostream<Char, Traits>& os,
//         const __quoted_output_proxy<Char, _Iter, Traits> & __proxy)
//{
//    return __quoted_output (os, __proxy.first, __proxy.last, __proxy.delim, __proxy.escape);
//}
//
//template <typename Char, typename Traits, typename Allocator>
//struct __quoted_proxy
//{
//    basic_string<Char, Traits, Allocator> &string;
//    Char  delim;
//    Char  escape;
//
//    __quoted_proxy(basic_string<Char, Traits, Allocator> &__s, Char __d, Char __e)
//    : string(__s), delim(__d), escape(__e) {}
//};
//
//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//basic_ostream<Char, Traits>& operator<<(
//        basic_ostream<Char, Traits>& os,
//        const __quoted_proxy<Char, Traits, Allocator> & __proxy)
//{
//    return __quoted_output (os, __proxy.string.cbegin (), __proxy.string.cend (), __proxy.delim, __proxy.escape);
//}
//
////  extractor for non-const basic_string& proxies
//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//basic_istream<Char, Traits>& operator>>(
//        basic_istream<Char, Traits>& is,
//        const __quoted_proxy<Char, Traits, Allocator> & __proxy)
//{
//    return quoted_input ( is, __proxy.string, __proxy.delim, __proxy.escape );
//}
//
//
//template <typename Char>
//_LIBCPP_INLINE_VISIBILITY
//__quoted_output_proxy<Char, const Char *>
//quoted ( const Char *__s, Char delim = Char('"'), Char escape =Char('\\'))
//{
//    const Char *__end = __s;
//    while ( *__end ) ++__end;
//    return __quoted_output_proxy<Char, const Char *> ( __s, __end, delim, escape );
//}
//
//
//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//__quoted_output_proxy<Char, typename basic_string <Char, Traits, Allocator>::const_iterator>
//__quoted ( const basic_string <Char, Traits, Allocator> &__s, Char delim = Char('"'), Char escape=Char('\\'))
//{
//    return __quoted_output_proxy<Char,
//            typename basic_string <Char, Traits, Allocator>::const_iterator>
//                    ( __s.cbegin(), __s.cend (), delim, escape );
//}
//
//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//__quoted_proxy<Char, Traits, Allocator>
//__quoted ( basic_string <Char, Traits, Allocator> &__s, Char delim = Char('"'), Char escape=Char('\\'))
//{
//    return __quoted_proxy<Char, Traits, Allocator>( __s, delim, escape );
//}

// FUNCTIONS
// ---------

//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//__quoted_output_proxy<Char, typename basic_string <Char, Traits, Allocator>::const_iterator>
//quoted ( const basic_string <Char, Traits, Allocator> &__s, Char delim = Char('"'), Char escape=Char('\\'))
//{
//    return __quoted(__s, delim, escape);
//}
//
//template <typename Char, typename Traits, typename Allocator>
//_LIBCPP_INLINE_VISIBILITY
//__quoted_proxy<Char, Traits, Allocator>
//quoted ( basic_string <Char, Traits, Allocator> &__s, Char delim = Char('"'), Char escape=Char('\\'))
//{
//    return __quoted(__s, delim, escape);
//}
//
//template <typename Char, typename Traits>
//__quoted_output_proxy<Char, const Char *, Traits>
//quoted (basic_string_view <Char, Traits> __sv,
//             Char delim = Char('"'), Char escape=Char('\\'))
//{
//    return __quoted_output_proxy<Char, const Char *, Traits>
//         ( __sv.data(), __sv.data() + __sv.size(), delim, escape );
//}

#endif                              // CPP14

PYCPP_END_NAMESPACE
