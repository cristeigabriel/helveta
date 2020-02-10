/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian
*contributors: @T0b1-iOS + @mooncaller + Laxol
*licensing: MIT License

*file description: internal and external memory handler
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <memory>
#include <windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

namespace helveta {

namespace memory {

// windows api read wrapper
inline bool read(const HANDLE         process_handle,
                 const std::uintptr_t read_location, void *buffer,
                 const std::size_t buffer_size) {

  return ReadProcessMemory(process_handle,
                           reinterpret_cast<LPCVOID>(read_location), buffer,
                           buffer_size, nullptr);
}

// windows api write wrapper
inline bool write(const HANDLE         process_handle,
                  const std::uintptr_t write_location,
                  const void *data_to_write, const std::size_t data_size) {

  return WriteProcessMemory(process_handle,
                            reinterpret_cast<LPVOID>(write_location),
                            data_to_write, data_size, nullptr);
}

template <typename type>
bool read(const HANDLE process_handle, const std::uintptr_t address,
          type &var) {

  return read(process_handle, address, std::addressof(var), sizeof(type));
}

// read_memory not null
template <typename type>
bool read_nn(const HANDLE process_handle, const std::uintptr_t address,
             type &var) {

  return read(process_handle, address, std::addressof(var), sizeof(type)) &&
         (var != 0);
}

template <typename type>
bool write(const HANDLE process_handle, const std::uintptr_t address,
           const type &var) {

  return write(process_handle, address, std::addressof(var), sizeof(type));
}

/**
 * \brief Finds a signature in a data range
 * \param data pointer to array of bytes that is to be searched
 * \param size size of the data array in bytes
 * \param sig pointer to array of bytes that is searched for
 * \param sig_len size of sig array in bytes
 * \param mask pointer to array of bools that controls whether a byte in the signature is compared
 * \return offset from data start when found, otherwise nothing
 */
inline std::optional<std::uintptr_t> find_sig_in_data(const std::uint8_t *data,
                                                      const std::size_t   size,
                                                      const std::uint8_t *sig,
                                                      const size_t sig_len,
                                                      const bool * mask) {
  const auto it = std::search(
      data, data + size,
      std::default_searcher(sig, sig + sig_len,
                            [sig, mask](const auto b1, const auto &b2) {
                              return (!mask[&b2 - &sig[0]]) || (b1 == b2);
                            }));

  if (it == (data + size)) return {};

  return (it - data);
}

/**
 * \brief Finds a signature in a data range
 * \tparam data_arr_type array type that implements begin/end
 * \tparam sig_arr_type array type that implements begin/end and ::value_type
 * (should ideally be the same type as the data or at least the same element byte size)
 * \tparam mask_arr_type array type that implements operator[]
 * \param data array of data that is to be searched
 * \param sig array of data that is searched for
 * \param mask array of type that is comparable to 0
 * \return offset from data start when found, otherwise nothing
 */
template <typename data_arr_type, typename sig_arr_type, typename mask_arr_type>
std::optional<std::uintptr_t> find_sig_in_data(const data_arr_type &data,
                                               const sig_arr_type & sig,
                                               const mask_arr_type &mask) {
  const auto it = std::search(
      data.begin(), data.end(),
      std::default_searcher(
          sig.begin(), sig.end(), [sig, mask](const auto b1, const auto &b2) {
            return (!mask[(&b2 - &sig[0]) /
                          sizeof(typename sig_arr_type::value_type)]) ||
                   (b1 == b2);
          }));

  if (it == data.end()) return {};

  return (it - data.begin());
}

/**
 * \brief Finds a signature in a data range ignoring zero-bytes in the signature
 * \param data pointer to array of bytes that is to be searched
 * \param size size of the data array in bytes
 * \param sig pointer to array of bytes that is searched for
 * \param sig_len size of sig array in bytes
 * \return offset from data start when found, otherwise nothing
 */
inline std::optional<std::uintptr_t> find_sig_in_data(const std::uint8_t *data,
                                                      const std::size_t   size,
                                                      const std::uint8_t *sig,
                                                      const size_t sig_len) {
  const auto it =
      std::search(data, data + size,
                  std::default_searcher(sig, sig + sig_len,
                                        [](const auto b1, const auto b2) {
                                          return (b1 == b2) || (b2 == 0u);
                                        }));

  if (it == (data + size)) return {};

  return (it - data);
}

/**
 * \brief Finds a signature in a data range ignoring zero-bytes in the signature
 * \tparam data_arr_type array type that implements begin/end
 * \tparam sig_arr_type array type that implements begin/end
 * \param data array of data that is to be searched
 * \param sig array of data that is searched for
 * \return offset from data start when found, otherwise nothing
 */
template <typename data_arr_type, typename sig_arr_type>
std::optional<std::uintptr_t> find_sig_in_data(const data_arr_type &data,
                                               const sig_arr_type & sig) {
  const auto it =
      std::search(data.begin(), data.end(),
                  std::default_searcher(sig.begin(), sig.end(),
                                        [](const auto b1, const auto b2) {
                                          return (b1 == b2) || (b2 == 0u);
                                        }));

  if (it == data.end()) return {};

  return (it - data.begin());
}

/**
 * \brief Finds a signature in a data range using a code-style pattern
 * \param data pointer to array of bytes that is to be searched
 * \param size size of the data array in bytes
 * \param sig string_view that contains the bytes to be searched for (can contain zero-bytes)
 * \param mask mask that controls whether a byte in the sig should be compared ('?' means ignore byte)
 * \return offset from data start when found, otherwise nothing
 */
inline std::optional<std::uintptr_t>
find_sig_in_data(const std::uint8_t *data, const std::size_t size,
                 const std::string_view sig, const std::string_view mask) {
  const auto it = std::search(
      data, data + size,
      std::default_searcher(sig.begin(), sig.end(),
                            [sig, mask](const auto b1, const auto &b2) {
                              return (mask[&b2 - &sig[0]] == '?') || (b1 == b2);
                            }));

  if (it == (data + size)) return {};

  return (it - data);
}

/**
 * \brief Finds a signature in a data range using a code-style pattern
 * \tparam data_arr_type array type that implements .data(), .size() and ::value_type
 * \param data array of data that is to be searched
 * \param sig string_view that contains the bytes to be searched for (can contain zero-bytes)
 * \param mask mask that controls whether a byte in the sig should be compared ('?' means ignore byte)
 * \return offset from data start when found, otherwise nothing
 */
template <typename data_arr_type>
std::optional<std::uintptr_t> find_sig_in_data(const data_arr_type &  data,
                                               const std::string_view sig,
                                               const std::string_view mask) {
  return find_sig_in_data(
      reinterpret_cast<const std::uint8_t *>(data.data()),
      data.size() * sizeof(typename data_arr_type::value_type), sig, mask);
}

} // namespace memory
} // namespace helveta
