//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Wrapper of the fast xxHash algorithm as an STL hash function.
 *
 *  Wrapper of the xxHash algorithm to be a drop-in replacement for
 *  `std::hash`. The STL hash function differs compiler to compiler,
 *  and although some compilers use the fairly fast MurmurHash2,
 *  other compilers such as MSVC use the glacial FNV 1a hash function.
 *  xxHash is faster than all existing STL hash functions, at the cost
 *  of some additional memory overhead [1].
 *
 *  1. https://github.com/Cyan4973/xxHash
 *
 *  \synopsis
 *      using hash_result_t = implementation-defined;
 *      static constexpr hash_result_t HASH_SEED = implementation-defined;
 *
 *      #define PYCPP_SPECIALIZE_HASH_STRING(name, type)    implementation-defined
 *
 *      hash_result_t hash_string(const void* buffer, size_t size) noexcept;
 *
 *      template <typename T>
 *      struct hash;
 */

#pragma once

#include <pycpp/preprocessor/architecture.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional/hash_specialize.h>
#include <pycpp/stl/functional/xxhash_c.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// These numbers were generated using NumPy with an initial seed
// of 0.
// np.random.seed(0)
// np.random.randint(np.iinfo(np.int32).min, np.iinfo(np.int32).max)
// np.random.seed(0)
// np.random.randint(np.iinfo(np.int64).min, np.iinfo(np.int64).max)
#if PYCPP_SYSTEM_ARCHITECTURE <= 32
#   define PYCPP_USE_HASH32
#   define XXH_NO_LONG_LONG
    using hash_result_t = XXH32_hash_t;
    static constexpr hash_result_t HASH_SEED = 118409032;
#elif PYCPP_SYSTEM_ARCHITECTURE == 64
#   define PYCPP_USE_HASH64
    using hash_result_t = XXH64_hash_t;
    static constexpr hash_result_t HASH_SEED = 900450186894289455;
#else
#   error "Unsupported system architecture."
#endif

// FUNCTIONS
// ---------

inline
hash_result_t
hash_string(
    const void* buffer,
    size_t size
)
noexcept
{
#if defined(PYCPP_USE_HASH32)               // 32-bit
    return XXH32(buffer, size, HASH_SEED);
#elif defined(PYCPP_USE_HASH64)             // 64-bit
    return XXH64(buffer, size, HASH_SEED);
#else                                       // Unsupported
#   error "Unsupported system architecture."
#endif                                      // Hash size
}

// SPECIALIZATION
// --------------

/**
 *  Specialize hashes for string types.
 */
#define PYCPP_SPECIALIZE_HASH_STRING(name, parameters, type)                                \
    template <typename T>                                                                   \
    struct name;                                                                            \
                                                                                            \
    template <parameters>                                                                   \
    struct name<type>                                                                       \
    {                                                                                       \
        using argument_type = type;                                                         \
        using result_type = size_t;                                                         \
                                                                                            \
        size_t                                                                              \
        operator()(                                                                         \
            const argument_type& x                                                          \
        )                                                                                   \
        const noexcept                                                                      \
        {                                                                                   \
            using value_type = typename argument_type::value_type;                          \
            return PYSTD::hash_string(x.data(), x.size() * sizeof(value_type));   \
        }                                                                                   \
    }

PYCPP_SPECIALIZE_HASH_VALUE(hash, bool);
PYCPP_SPECIALIZE_HASH_VALUE(hash, char);
PYCPP_SPECIALIZE_HASH_VALUE(hash, signed char);
PYCPP_SPECIALIZE_HASH_VALUE(hash, unsigned char);
PYCPP_SPECIALIZE_HASH_VALUE(hash, char16_t);
PYCPP_SPECIALIZE_HASH_VALUE(hash, char32_t);
PYCPP_SPECIALIZE_HASH_VALUE(hash, wchar_t);
PYCPP_SPECIALIZE_HASH_VALUE(hash, short);
PYCPP_SPECIALIZE_HASH_VALUE(hash, unsigned short);
PYCPP_SPECIALIZE_HASH_VALUE(hash, int);
PYCPP_SPECIALIZE_HASH_VALUE(hash, unsigned int);
PYCPP_SPECIALIZE_HASH_VALUE(hash, long);
PYCPP_SPECIALIZE_HASH_VALUE(hash, long long);
PYCPP_SPECIALIZE_HASH_VALUE(hash, unsigned long);
PYCPP_SPECIALIZE_HASH_VALUE(hash, unsigned long long);
PYCPP_SPECIALIZE_HASH_VALUE(hash, float);
PYCPP_SPECIALIZE_HASH_VALUE(hash, double);
PYCPP_SPECIALIZE_HASH_VALUE(hash, long double);

// nullptr_t
template <>
struct hash<std::nullptr_t>
{
    using argument_type = std::nullptr_t;
    using result_type = size_t;

    size_t
    operator()(
        std::nullptr_t
    )
    const noexcept
    {
        return std::hash<uintptr_t>()(0);
    }
};

// Pointer
template <typename T>
struct hash<T*>
{
    using argument_type = T*;
    using result_type = size_t;

    size_t
    operator()(
        T* x
    )
    const noexcept
    {
        return std::hash<T*>()(x);
    }
};

// OBJECTS
// -------

// ENUM HASH

// Enum
template <typename T, bool = std::is_enum<T>::value>
struct enum_hash
{
    using argument_type = T;
    using result_type = size_t;

    size_t
    operator()(
        T x
    )
    const noexcept
    {
        using type = typename std::underlying_type<T>::type;
        return std::hash<type>()(static_cast<type>(x));
    }
};

template <typename T>
struct enum_hash<T, false>
{
    enum_hash() = delete;
    enum_hash(const enum_hash&) = delete;
    enum_hash& operator=(const enum_hash&) = delete;
};

// HASH

template <typename T>
struct hash: public enum_hash<T>
{};

// CLEANUP
// -------

#undef PYCPP_USE_HASH32
#undef PYCPP_USE_HASH64

PYCPP_END_NAMESPACE
