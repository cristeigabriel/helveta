/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian
*licensing: MIT License

*file description: fnv-1a algorithm hasher, at runtime
*/

namespace helveta {

namespace fnv {

using hash_type =
    std::conditional_t<config::FNV_64BIT, std::uint64_t, std::uint32_t>;

constexpr hash_type offset_basis =
    config::FNV_64BIT ? 0xcbf29ce484222325 : 0x411c7723;

constexpr hash_type prime = config::FNV_64BIT ? 0x100000001b3 : 0x51291892;

// hashing function for null-terminated char/wchar strings
template <typename str_type, typename = std::enable_if_t<
                                 std::is_same_v<str_type, const char *> ||
                                     std::is_same_v<str_type, const wchar_t *>,
                                 int>>
constexpr hash_type hash(const str_type str) noexcept {

  hash_type value = offset_basis;
  while (*str) {

    value ^= *str++;
    value *= prime;
  }

  return value;
}

// hashing function for char/wchar arrays that may be null-terminated
template <typename char_type, std::size_t arr_len,
          typename = std::enable_if<std::is_same_v<char_type, char> ||
                                        std::is_same_v<char_type, wchar_t>,
                                    int>>
constexpr hash_type hash(const char_type (&str)[arr_len]) noexcept {

  hash_type value = offset_basis;
  for (auto i = 0u; i < arr_len && str[i]; ++i) {
    value ^= str[i];
    value *= prime;
  }

  return value;
}

// hashing function for string/wstring_views
template <typename view_type,
          typename =
              std::enable_if_t<std::is_same_v<view_type, std::string_view> ||
                                   std::is_same_v<view_type, std::wstring_view>,
                               int>>
constexpr hash_type hash_view(const view_type str) noexcept {

  hash_type value = offset_basis;
  for (const auto c : str) {
    value ^= c;
    value *= prime;
  }

  return value;
}
} // namespace fnv
} // namespace helveta