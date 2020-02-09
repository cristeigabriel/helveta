/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for
projects *written by: Cristei Gabriel
*file written by: T0b1-iOS
*licensing: MIT License

*file description: external pointer wrapper
*/

// Code Note: so if you don't know templates you should probably not try to
// change anything in here if you do I gladly welcome any simplification of the
// templates
namespace helveta {

namespace memory {

namespace detail {
template <typename derived_type,
          typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
class crtp_ptr_base {
public:
  using pointer_type = std::conditional_t<x64, std::uint64_t, std::uint32_t>;

  crtp_ptr_base() {}

  explicit crtp_ptr_base(const pointer_type addr) : _addr(addr) {}

  derived_type &add(const size_t count) {
    _addr += sizeof(type) * count;
    return static_cast<derived_type &>(*this);
  }

  derived_type &sub(const size_t count) {
    _addr -= sizeof(type) * count;
    return static_cast<derived_type &>(*this);
  }

  derived_type &add_bytes(const size_t count) {
    _addr += count;
    return static_cast<derived_type &>(*this);
  }

  derived_type &sub_bytes(const size_t count) {
    _addr -= count;
    return static_cast<derived_type &>(*this);
  }

  // TODO: make it "const type val" for fundamental types?
  bool write(const HANDLE handle, const type &val) {
    return memory::write(handle, this->_addr, val);
  }

  bool read(const HANDLE handle, type &val) const {
    if (!this->_addr) return false;

    return memory::read(handle, this->_addr, val);
  }

  type read_val(const HANDLE handle) const {
    if (!this->_addr) return type{};

    type val;
    memory::read(handle, this->_addr, val);

    return val;
  }

  derived_type &deref(const HANDLE handle) {
    if (!this->_addr) return;

    memory::read(handle, this->_addr, this->_addr);
    return static_cast<derived_type &>(*this);
  }

  derived_type &deref(const HANDLE handle, const std::size_t count) {
    for (auto i = 0u; i < count && this->_addr; ++i) deref(handle);
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator+(const std::size_t count) {
    add(count);
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator+=(const std::size_t count) {
    add(count);
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator-(const std::size_t count) {
    sub(count);
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator-=(const std::size_t count) {
    sub(count);
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator=(const derived_type &ptr) {
    _addr = ptr._addr;
    return static_cast<derived_type &>(*this);
  }

  derived_type &operator=(const type *&ptr) {
    _addr = reinterpret_cast<pointer_type>(ptr);
    return static_cast<derived_type &>(*this);
  }

  bool operator==(const pointer_type val) const { return (_addr == val); }
  bool operator!=(const pointer_type val) const { return (_addr != val); }

  bool operator!() const { return (_addr != 0u); }

  pointer_type addr() const { return _addr; }

protected:
  pointer_type _addr = 0u;
};

template <typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
class base_ptr : public crtp_ptr_base<base_ptr<type, x64>, type, x64> {
  using base = crtp_ptr_base<base_ptr<type, x64>, type, x64>;

public:
  base_ptr() : base{} {}

  explicit base_ptr(const typename base::pointer_type addr) : base{addr} {}
};

template <typename derived_type,
          typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
class crtp_global_ptr_base : public crtp_ptr_base<derived_type, type, x64> {
  using base = crtp_ptr_base<derived_type, type, x64>;

public:
  crtp_global_ptr_base() : base{} {}

  explicit crtp_global_ptr_base(const typename base::pointer_type addr)
      : base{addr} {}

  // TODO: make it "const type val" for fundamental types?
  bool write(const type &val) {
    return process::get<true>().write(this->_addr, val);
  }

  bool read(type &val) const {
    if (!this->_addr) return false;

    return process::get<true>().read(this->_addr, val);
  }

  type read_val() const {
    if (!this->_addr) return type{};

    type val;
    process::get<true>().read(this->_addr, val);

    return val;
  }

  derived_type &deref() {
    if (!this->_addr) return;

    process::get<true>().read(this->_addr, this->_addr);
    return static_cast<derived_type &>(*this);
  }

  derived_type &deref(const std::size_t count) {
    for (auto i = 0u; i < count && this->_addr; ++i) deref();
    return static_cast<derived_type &>(*this);
  }

  type operator*() const { return read_val(); }

  type operator[](const std::size_t idx) const {
    if (!this->_addr) return type{};

    typename base::pointer_type arr_addr;
    process::get<true>().read(this->_addr, arr_addr);
    if (!arr_addr) return type{};

    const auto addr = arr_addr + (sizeof(type) * idx);

    type val;
    process::get<true>().read(this->_addr, val);

    return val;
  }

  derived_type &operator=(const type &val) {
    write(val);
    return static_cast<derived_type &>(*this);
  }
};

template <typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
class global_ptr
    : public crtp_global_ptr_base<global_ptr<type, x64>, type, x64> {
  using base = crtp_global_ptr_base<global_ptr<type, x64>, type, x64>;

public:
  global_ptr() : base{} {}

  explicit global_ptr(const typename base::pointer_type addr) : base{addr} {}

  // i dont understand why i need this here...
  global_ptr<type, x64> &operator=(const type &val) {
    write(val);
    return *this;
  }
};

template <typename derived_type,
          typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
using crtp_ptr =
    std::conditional_t<config::GLOBAL_PROCESS_INSTANCE,
                       crtp_global_ptr_base<derived_type, type, x64>,
                       crtp_ptr_base<derived_type, type, x64>>;
} // namespace detail

template <typename type = std::conditional_t<config::PTR_DEF_64BIT,
                                             std::uint64_t, std::uint32_t>,
          bool x64      = config::PTR_DEF_64BIT>
using ptr = std::conditional_t<config::GLOBAL_PROCESS_INSTANCE,
                               detail::global_ptr<type, x64>,
                               detail::base_ptr<type, x64>>;

namespace detail {
// so that template template works but it looks really sketchy
template <typename derived_type, typename char_type,
          bool x64 = config::PTR_DEF_64BIT,
          template <typename, typename = char_type, bool = x64>
          typename ptr_base = crtp_ptr,
          typename = std::enable_if_t<std::is_same_v<char_type, char> ||
                                          std::is_same_v<char_type, wchar_t>,
                                      int>>
class crtp_str_ptr_base : public ptr_base<derived_type, char_type, x64> {
  using base = ptr_base<derived_type, char_type, x64>;

public:
  using string = std::basic_string<char_type, std::char_traits<char_type>,
                                   std::allocator<char_type>>;

  crtp_str_ptr_base() : base{} {}

  explicit crtp_str_ptr_base(const std::uintptr_t addr) : base{addr} {}

  string read_str(const HANDLE      handle,
                  const std::size_t max_str_size = 500) const {
    return read_str_impl(handle, max_str_size);
  }

  bool read_str_buf(const HANDLE handle, char_type *buf,
                    const std::size_t size) const {
    return memory::read(handle, this->_addr, buf, size * sizeof(char_type));
  }

  template <std::size_t len>
  bool read_str_buf(const HANDLE handle, char_type (&buf)[len]) const {
    return read_str_buf(handle, buf, len);
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
      if (!memory::read(handle, this->_addr + off, buf,
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

template <typename char_type, bool x64 = config::PTR_DEF_64BIT,
          typename = std::enable_if_t<std::is_same_v<char_type, char> ||
                                          std::is_same_v<char_type, wchar_t>,
                                      int>>
class basic_str_ptr : public crtp_str_ptr_base<basic_str_ptr<char_type, x64>,
                                               char_type, x64, crtp_ptr_base> {
  using base = crtp_str_ptr_base<basic_str_ptr<char_type, x64>, char_type, x64,
                                 crtp_ptr_base>;

public:
  basic_str_ptr() : base{} {}

  explicit basic_str_ptr(const std::uintptr_t addr) : base{addr} {}
};

template <typename char_type, bool x64 = config::PTR_DEF_64BIT,
          typename = std::enable_if_t<std::is_same_v<char_type, char> ||
                                          std::is_same_v<char_type, wchar_t>,
                                      int>>
class global_str_ptr
    : public crtp_str_ptr_base<global_str_ptr<char_type, x64>, char_type, x64,
                               crtp_global_ptr_base> {
public:
  using base_ptr = crtp_str_ptr_base<global_str_ptr<char_type, x64>, char_type,
                                     x64, crtp_global_ptr_base>;
  using string   = typename base_ptr::string;

  global_str_ptr() : base_ptr{} {}

  explicit global_str_ptr(const std::uintptr_t addr) : base_ptr{addr} {}

  string read_str(const std::size_t max_str_size = 500) const {
    return this->read_str_impl(process::get<true>().handle(), max_str_size);
  }

  bool read_str_buf(char_type *buf, const std::size_t size) const {
    return memory::read(process::get<true>().handle(), this->_addr, buf,
                        size * sizeof(char_type));
  }

  template <std::size_t len>
  bool read_str_buf(char_type (&buf)[len]) const {
    return read_str_buf(buf, len);
  }

  string operator*() const { return read_str(); }
};
} // namespace detail

template <bool x64 = config::PTR_DEF_64BIT>
using str_ptr = std::conditional_t<config::GLOBAL_PROCESS_INSTANCE,
                                   detail::global_str_ptr<char, x64>,
                                   detail::basic_str_ptr<char, x64>>;

template <bool x64 = config::PTR_DEF_64BIT>
using wstr_ptr = std::conditional_t<config::GLOBAL_PROCESS_INSTANCE,
                                    detail::global_str_ptr<wchar_t, x64>,
                                    detail::basic_str_ptr<wchar_t, x64>>;

} // namespace memory
} // namespace helveta
