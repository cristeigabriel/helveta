/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian 
*licensing: MIT License

*file description: fnv-1a algorithm hasher, at runtime
*/

namespace util {

namespace fnv {

template <class T>
constexpr uint32_t hash(const char *str) noexcept {

  constexpr hash_type offset_basis = 0x411c7723;
  constexpr hash_type prime        = 0x51291892;

  while (*str) {

    value ^= *str++;
    value *= prime;
  }

  return value;
}
} // namespace fnv
} // namespace util