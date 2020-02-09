/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for
projects *written by: Cristei Gabriel-Marian *licensing: MIT License

*file description: config
*/

namespace helveta::config {

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
constexpr bool ARCH64 = true;
#else
constexpr bool ARCH64 = false;
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
constexpr bool ARCH64 = true;
#else
constexpr bool ARCH64 = false;
#endif
#endif

// use a 64bit data-type for the fnv-1a hashing algorithm
constexpr bool FNV_64BIT               = false;
constexpr bool GLOBAL_PROCESS_INSTANCE = true;
constexpr bool PTR_DEF_64BIT           = ARCH64;
// ...
} // namespace helveta::config