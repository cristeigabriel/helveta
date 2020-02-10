/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel 
*file written by: T0b1-iOS, @cristeigabriel
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
public:
  struct module_t {
    fnv::hash_type name_hash;
    std::uintptr_t addr;
    std::size_t    size;
  };

  process() = default;
  ~process() {

    if (_handle && !_custom_handle) CloseHandle(_handle);
  }

  /**
   * \brief Attach to a process with a user-provided handle
   * When this function is used to attach the handle will not be closed automatically when detached
   * \param handle Handle to process
   * \param id PID of process
   */
  void attach(const HANDLE handle, const std::uint32_t id) {

    _custom_handle = true;
    _handle        = handle;
    _id            = id;
    update_arch();
  }

  /**
   * \brief Attach to a process using its PID
   * \param id PID of process
   * \return true when attach succeeded
   */
  bool attach(const std::uint32_t id) {

    _handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
                              PROCESS_VM_READ | PROCESS_VM_WRITE,
                          false, id);

    if (!_handle) return false;

    _id = id;

    update_arch();
    return true;
  }

  /**
   * \brief Attach to a process using its name hash
   * \param name_hash FNV-1A Hash of the process' name
   * \return true when attach succeeded
   */
  bool attach_hash(fnv::hash_type name_hash);

  /**
   * \brief Attach to a process using its name
   * \param name Name of the process
   * \return true when attach succeeded
   */
  bool attach(const std::string_view name) {
    return attach_hash(fnv::hash_view(name));
  }

  void detach() {

    if (_handle && !_custom_handle) CloseHandle(_handle);
    _custom_handle = false;
    _handle        = nullptr;
    _id            = 0u;
    _x64           = false;
    _mod_list.clear();
  }

  /**
   * \brief Find a module using its name hash
   * \param name_hash  FNV-1A Hash of the modules' name
   * \return module_t info structure when found otherwise nothing
   */
  std::optional<module_t> find_module(fnv::hash_type name_hash);

  /**
   * \brief Find a module using its name
   * \param name Name of the module
   * \return module_t info structure when found otherwise nothing
   */
  std::optional<module_t> find_module(const std::string_view name) {
    return find_module(fnv::hash_view(name));
  }

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
    return read(address, std::addressof(var), sizeof(type));
  }

  // read not null
  template <typename type>
  bool read_nn(const std::uintptr_t address, type &var) const {
    return read(address, std::addressof(var), sizeof(type)) && (var != 0);
  }

  template <typename type,
            typename = std::enable_if_t<!std::is_fundamental_v<type>, int>>
  bool write(const std::uintptr_t address, const type &var) const {
    return write(address, std::addressof(var), sizeof(type));
  }

  template <typename type,
            typename = std::enable_if_t<std::is_fundamental_v<type>, int>>
  bool write(const std::uintptr_t address, const type var) const {
    return write(address, std::addressof(var), sizeof(type));
  }

  // getter
  HANDLE        handle() const noexcept { return _handle; }
  std::uint32_t id() const noexcept { return _id; }
  bool          x64() const noexcept { return _x64; }

  const auto &module_list() const noexcept { return _mod_list; }

  template <bool enable = config::GLOBAL_PROCESS_INSTANCE>
  static process &get(typename std::enable_if_t<enable, int> = 0) {
    static process inst{};
    return inst;
  }

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
