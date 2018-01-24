//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Scoped class that saves stream flagss.
 *
 *  \synopsis
 *      template <typename Char, typename Traits>
 *      class save_flags
 *      {
 *      public:
 *          explicit save_flags(stream_type& stream);
 *          save_flags(const save_flags&) = delete;
 *          save_flags& operator=(const save_flags&) = delete;
 *          ~save_flags();
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <ios>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

template <typename Char, typename Traits>
class save_flags
{
    using stream_type = std::basic_ios<Char, Traits>;
    using fmtflags = typename stream_type::fmtflags;

    stream_type& stream_;
    fmtflags fmtflags_;
    Char fill_;

public:
    explicit
    save_flags(
        stream_type& stream
    )
    :
        stream_(stream),
        fmtflags_(stream.flags()),
        fill_(stream.fill())
    {}

    save_flags(const save_flags&) = delete;
    save_flags& operator=(const save_flags&) = delete;

    ~save_flags()
    {
        stream_.flags(fmtflags_);
        stream_.fill(fill_);
    }
};

PYCPP_END_NAMESPACE
