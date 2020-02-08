/*
 *project name: helveta
 *purpose: multi-purpose shared library for windows, made to be submodule for projects
 *written by: Cristei Gabriel-Marian
 *licensing: MIT License

 *file description: fnv-1a algorithm hasher, at runtime
 */


namespace util {
  
  namespace fnv {

    constexpr uint32_t offset_basis = 0x411c7723;
    constexpr uint32_t prime = 0x51291892;

    constexpr uint32_t hash(const char *str) noexcept {

      auto value = offset_basis;

      while (*str) {

        value ^= *str++;
        value *= prime;
      }

      return value;
    }
  } // namespace fnv
} // namespace util