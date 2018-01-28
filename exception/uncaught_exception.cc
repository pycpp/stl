//  :copyright: (c) 2007-2015 Andrey Semashev.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/preprocessor/abi.h>
#include <pycpp/stl/exception/uncaught_exception.h>

#if !defined(PYCPP_CPP17)           // <=CPP14

// MACROS
// ------

#if defined(PYCPP_CXXABI)
// MinGW GCC 4.4 seem to not work the same way the newer GCC versions do.
// As a result, __cxa_get_globals based implementation will always return 0.
// Just disable it for now and fall back to std::uncaught_exception().
#   if !defined(__MINGW32__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)))
// Only GCC 4.7 declares __cxa_get_globals() in cxxabi.h, older compilers do not expose this function but it's there
#       define PYCPP_CXA_GET_GLOBALS

extern "C" void* __cxa_get_globals();

#   endif
#elif defined(PYCPP_MSVC)
#   if PYCPP_COMPILER_MAJOR_VERSION >= 14
    // Visual Studio 14 supports N4152 std::uncaught_exceptions()
#       define PYCPP_UNCAUGHT_EXCEPTIONS
#   elif PYCPP_COMPILER_MAJOR_VERSION >= 8
#       define PYCPP_GETPTD

extern "C" void* _getptd();

#   endif
#endif

// HELPERS
// -------

// Returns the number of currently pending exceptions
static inline
unsigned int
uncaught_exception_count()
noexcept
{
#if defined(PYCPP_UNCAUGHT_EXCEPTIONS)
    // C++17 implementation, available in Visual studio 14.
    return static_cast<unsigned int>(std::uncaught_exceptions());
#elif defined(PYCPP_CXA_GET_GLOBALS)
    // Tested on {clang 3.2,GCC 3.5.6,GCC 4.1.2,GCC 4.4.6,GCC 4.4.7}x{x32,x64}
    // __cxa_eh_globals::uncaughtExceptions, x32 offset - 0x4, x64 - 0x8
    return *(reinterpret_cast< const unsigned int* >(static_cast< const char* >(__cxa_get_globals()) + sizeof(void*)));
#elif defined(PYCPP_GETPTD)
    // MSVC specific. Tested on {MSVC2005SP1,MSVC2008SP1,MSVC2010SP1,MSVC2012}x{x32,x64}.
    // _tiddata::_ProcessingThrow, x32 offset - 0x90, x64 - 0x100
    return *(reinterpret_cast< const unsigned int* >(static_cast< const char* >(_getptd()) + (sizeof(void*) == 8 ? 0x100 : 0x90)));
#else
    // Portable implementation. Does not allow to detect multiple nested exceptions.
    return static_cast<unsigned int>(std::uncaught_exception());
#endif
}

// FUNCTIONS
// ---------

int
uncaught_exceptions()
noexcept
{
    return static_cast<int>(uncaught_exception_count());
}

#endif                              // <=CPP14
