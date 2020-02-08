/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian 
*licensing: MIT License

*file description: internal and external memory handler
*/

#include <Windows.h>

namespace util {

namespace memory {

namespace internal {

template <typename T> T read(std::uintptr_t const addr) noexcept {

  return *reinterpret_cast<T *>(addr);
}

template <typename T> T write(std::uintptr_t const addr, T val) noexcept {

  return *reinterpret_cast<T *>(addr) = val;
}
} // namespace internal

namespace external {

const void *read(const HANDLE &process_handle,
                 const std::uintptr_t &read_location,
                 const std::uintptr_t &read_size) noexcept(false) {

  void *read_value;

  if (!ReadProcessMemory(process_handle, (void *)read_location, &read_value,
                         read_size, 0)) {

    return 0;
  }
}

bool write(const HANDLE &process_handle, const std::uintptr_t &write_location,
           const void *&data_to_write,
           const std::size_t &data_size) noexcept(false) {

  bool write_successful = WriteProcessMemory(
      process_handle, (void *)write_location, data_to_write, data_size, 0);

  return write_successful;
}
} // namespace external
} // namespace memory
} // namespace util