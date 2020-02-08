/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian + Laxol 
*contributors: @T0b1-iOS
*licensing: MIT License

*file description: internal and external memory handler
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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

  return read(process_handle, address, &var, sizeof(type));
}

// read_memory not null
template <typename type>
bool read_nn(const HANDLE process_handle, const std::uintptr_t address,
             type &var) {

  return read(process_handle, address, &var, sizeof(type)) && (var != 0);
}

template <typename type>
bool write(const HANDLE process_handle, const std::uintptr_t address,
           const type &var) {

  return write(process_handle, address, &var, sizeof(type));
}
} // namespace memory
} // namespace helveta
