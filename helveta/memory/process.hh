/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel
*file written by: T0b1-iOS
*licensing: MIT License

*file description: external process wrapper
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <string_view>
#include <windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

namespace helveta {
namespace memory {
class process {
  struct module_t {
    std::uint32_t  name_hash;
    std::uintptr_t addr;
    std::size_t    size;
  };

public:
  process() = default;
  ~process() {
    if (_handle && !_custom_handle) CloseHandle(_handle);
  }

  void attach(const HANDLE handle, const std::uint32_t id) {
    _custom_handle = true;
    _handle        = handle;
    _id            = id;
    update_arch();
  }

  bool attach(const std::uint32_t id) {
    _handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
                              PROCESS_VM_READ | PROCESS_VM_WRITE,
                          false, id);

    if (!_handle) return false;

    _id = id;

    update_arch();
    return true;
  }

  bool attach(const std::string_view name);

  void detach() {
    if (_handle && !_custom_handle) CloseHandle(_handle);
    _custom_handle = false;
    _handle        = nullptr;
    _id            = 0u;
    _x64           = false;
    _mod_list.clear();
  }

  [[nodiscard]] std::uintptr_t find_module(std::string_view name);

  // Memory Operations

  bool read(const std::uintptr_t read_location, void *buffer,
            const std::size_t buffer_size) const {

    return ReadProcessMemory(_handle, reinterpret_cast<LPCVOID>(read_location),
                             buffer, buffer_size, nullptr);
  }

  bool write(const std::uintptr_t write_location, const void *data_to_write,
             const std::size_t data_size) const {

    return WriteProcessMemory(_handle, reinterpret_cast<LPVOID>(write_location),
                              data_to_write, data_size, nullptr);
  }

  template <typename type>
  bool read(const std::uintptr_t address, type &var) const {
    return read(address, &var, sizeof(type));
  }

  // read not null
  template <typename type>
  bool read_nn(const std::uintptr_t address, type &var) const {
    return read(address, &var, sizeof(type)) && (var != 0);
  }

  template <typename type>
  bool write(const std::uintptr_t address, const type &var) const {
    return write(address, &var, sizeof(type));
  }

  // getter
  [[nodiscard]] HANDLE        handle() const noexcept { return _handle; }
  [[nodiscard]] std::uint32_t id() const noexcept { return _id; }
  [[nodiscard]] bool          x64() const noexcept { return _x64; }

  [[nodiscard]] const auto &module_list() const noexcept { return _mod_list; }

private:
  void update_arch();
  void init_module_list();

  HANDLE        _handle        = nullptr;
  std::uint32_t _id            = 0u;
  bool          _x64           = false;
  bool          _custom_handle = false;

  std::vector<module_t> _mod_list{};
};
} // namespace memory
} // namespace helveta