//  :copyright: (c) 2007-2013 Andrey Semashev.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/stl/typeinfo/type_info_wrapper.h>
#if defined(HAVE_CXXABI)
// Avoid including <cxxabi.h> in header files.
// Can cause compatiblity issues with other source files.
#   include <cxxabi.h>
#   include <stdlib.h>
#endif

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

#if defined(HAVE_CXXABI)        // HAVE_CXXABI

// A simple scope guard for automatic memory free
struct auto_free
{
    explicit
    auto_free(
        void* p
    )
    noexcept:
        p_(p)
    {}

    ~auto_free()
    {
        free(p_);
    }

private:
    void* p_;
};

std::string
info_to_string(
    const std::type_info& info
)
{
    // GCC returns decorated type name, will need to demangle it using ABI
    int status = 0;
    size_t size = 0;
    const char* name = info.name();
    char* undecorated = abi::__cxa_demangle(name, NULL, &size, &status);
    auto_free cleanup(undecorated);

    if (undecorated) {
        return undecorated;
    } else {
        return name;
    }
}

#else                           // !HAVE_CXXABI

std::string
info_to_string(
    const std::type_info& info
)
{
    return info.name();
}

#endif                          // HAVE_CXXABI

PYCPP_END_NAMESPACE
