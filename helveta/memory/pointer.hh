/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects
*written by: Cristei Gabriel
*file written by: T0b1-iOS 
*licensing: MIT License

*file description: external pointer wrapper
*/

namespace helveta::memory {

template <typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
class ptr {
  using pointer_type = std::conditional_t<x64, std::uint64_t, std::uint32_t>;

public:
  ptr() {}

  explicit ptr(const pointer_type addr) : _addr(addr) {}

  void add(const size_t count) { _addr += sizeof(type) * count; }

  void sub(const size_t count) { _addr -= sizeof(type) * count; }

  void add_bytes(const size_t count) { _addr += count; }

  void sub_bytes(const size_t count) { _addr -= count; }

  // TODO: make it "const type val" for fundamental types?
  bool write(const type &val) { return process::get().write(_addr, val); }

  bool read(type &val) const {
    if (!_addr) return false;

    return process::get().read(_addr, val);
  }

  type read_val() const {
    if (!_addr) return type{};

    type val;
    process::get().read(_addr, val);

    return val;
  }

  void deref() {
    if (!_addr) return;

    process::get().read(_addr, _addr);
  }

  void deref(const std::size_t count) {
    for (auto i = 0u; i < count && _addr; ++i) deref();
  }

  type operator*() const { return read_val(); }

  ptr<type, x64> &operator+(const std::size_t count) {
    add(count);
    return *this;
  }

  ptr<type, x64> &operator+=(const std::size_t count) {
    add(count);
    return *this;
  }

  ptr<type, x64> &operator-(const std::size_t count) {
    sub(count);
    return *this;
  }

  ptr<type, x64> &operator-=(const std::size_t count) {
    sub(count);
    return *this;
  }

  ptr<type, x64> &operator*=(const type &val) {
    write(val);
    return *this;
  }

  ptr<type, x64> &operator=(const pointer_type addr) {
    _addr = addr;
    return *this;
  }

  bool operator==(const pointer_type val) const { return (_addr == val); }
  bool operator!=(const pointer_type val) const { return (_addr != val); }

  bool operator!() const { return (_addr != 0u); }

  type operator[](const std::size_t idx) const {
    const auto addr = _addr + (sizeof(type) * idx);
    if (!_addr) return type{};

    type val;
    process::get().read(_addr, val);

    return val;
  }

  pointer_type addr() const { return _addr; }

protected:
  pointer_type _addr = 0u;
};

template <typename char_type, bool x64 = config::PTR_DEF_64BIT,
          typename = std::enable_if_t<std::is_same_v<char_type, char> ||
                                          std::is_same_v<char_type, wchar_t>,
                                      int>>
class basic_str_ptr : ptr<char_type, x64> {

  using string = std::basic_string<char_type, std::char_traits<char_type>,
                                   std::allocator<char_type>>;

public:
  basic_str_ptr() : ptr{} {}

  explicit basic_str_ptr(const std::uintptr_t addr) : ptr{addr} {}

  template <typename = std::enable_if_t<!config::GLOBAL_PROCESS_INSTANCE, int>>
  string read_str(const HANDLE      handle,
                  const std::size_t max_str_size = 500) const {
    return read_str_impl(handle, max_str_size);
  }

  template <typename = std::enable_if_t<config::GLOBAL_PROCESS_INSTANCE, int>>
  string read_str(const std::size_t max_str_size = 500) const {
    return read_str_impl(process::get().handle(), max_str_size);
  }

  template <typename = std::enable_if_t<!config::GLOBAL_PROCESS_INSTANCE, int>>
  bool read_str_buf(const HANDLE handle, char_type *buf,
                    const std::size_t size) const {
    return memory::read(handle, _addr, buf, size);
  }

  template <typename = std::enable_if_t<config::GLOBAL_PROCESS_INSTANCE, int>>
  bool read_str_buf(char_type *buf, const std::size_t size) const {
    return memory::read(process::get().handle(), _addr, buf, size);
  }

  template <typename std::size_t len>
  bool read_str_buf(char_type (&buf)[len]) const {
    return read_str_buf(buf, len);
  }

  template <typename = std::enable_if_t<config::GLOBAL_PROCESS_INSTANCE, int>>
  string operator*() const {
    return read_str();
  }

protected:
  string read_str_impl(const HANDLE      handle,
                       const std::size_t max_str_size = 500) const {
    string val{};

    constexpr std::size_t buf_size = 51;
    char_type             buf[buf_size];
    buf[buf_size - 1u] = static_cast<char_type>(0);

    std::size_t off = 0u;

    while (true) {
      if (!memory::read(handle, _addr + off, buf,
                        sizeof(buf) - sizeof(char_type)))
        return val;

      const auto str_size = val.size();
      val.append(buf);
      if (val.size() - str_size < buf_size - 1) break;

      off += sizeof(buf) - 1u;
      if (off >= max_str_size) break;
    }

    return val;
  }
};

template <bool x64 = config::PTR_DEF_64BIT>
using str_ptr = basic_str_ptr<char, x64>;

template <bool x64 = config::PTR_DEF_64BIT>
using wstr_ptr = basic_str_ptr<wchar_t, x64>;

} // namespace helveta::memory
