/*

        *project name: helveta
        *purpose: multi-purpose shared library for windows, made to be submodule for projects
        *written by: Cristei Gabriel-Marian
        *licensing: MIT License
       
        *file description: the vector class used in helveta.
*/

namespace util {

template <class T> struct vec3_t {

  T x, y, z;

  static_assert(std::is_arithmetic<T>::value, "T must be numeric");

  constexpr vec3_t() : x(0), y(0), z(0) {}

  constexpr vec3_t(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) {}

  constexpr vec3_t(const vec3_t &vec) : x(vec.x), y(vec.y), z(vec.z) {}

  constexpr vec3_t(const vec3_t &vec, const vec3_t &vec_2) {}

  constexpr bool is_valid() noexcept(false) {

    // return validity state
    return std::isfinite(this->x) && std::isfinite(this->y) &&
           std::isfinite(this->z);
  }

  constexpr T size() noexcept {

    // return size
    return sizeof(vec3_t(this->x, this->y, this->z));
  }

  constexpr T length_squared() noexcept {

    // return length
    return sqrt((this->x * this->x) + (this->y * this->y));
  }

  constexpr T length() noexcept {

    // return length sqr
    return (this->x * this->x) + (this->y * this->y);
  }

  constexpr T distance(const vec3_t &rhs) noexcept {

    // calculate distance, returns base vector - argument vector to length
    return (*this - rhs).length();
  }

  constexpr vec3_t normalize() noexcept {

    // we return a new vector with our new values
    return vec3_t(this->x / length(), this->y / length(), this->z / length());
  }

  constexpr T dot(const vec3_t &rhs) noexcept {

    // calculate dot product of two vectors
    return (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);
  }

  constexpr T cross(const vec3_t &rhs) noexcept {

    // calculate cross product of two vectors
    return ((this->y * rhs.z) - (this->z * rhs.y),
            (this->z * rhs.x) - (this->x * rhs.z),
            (this->x * rhs.y) - (this->y * rhs.x));
  }

  constexpr bool empty() noexcept {

    // return as empty
    return this->x == 0 && this->y == 0 && this->z == 0;
  }

  constexpr bool equal(const vec3_t &rhs) noexcept {

    // return equality state
    return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
  }

  // handle adding
  constexpr vec3_t operator+(const vec3_t &rhs) noexcept {

    // return value handled as array of results into a new vector
    return vec3_t(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
  }

  // handle substracting
  constexpr vec3_t operator-(const vec3_t &rhs) noexcept {

    // return value handled as array of results into a new vector
    return vec3_t(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
  }

  // handle multiplying
  constexpr vec3_t operator*(const T &rhs) noexcept {

    // we return a new vector with our new values
    return vec3_t(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
  }

  // handle divisions
  constexpr vec3_t operator/(const T &rhs) noexcept {

    // we return a new vector with our new values
    return vec3_t(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
  }

  // handle adding to result
  const vec3_t &operator+=(const vec3_t &rhs) noexcept {

    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle substracting to result
  constexpr vec3_t &operator-=(const vec3_t &rhs) noexcept {

    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle multiplying to result
  constexpr vec3_t &operator*=(const T &rhs) noexcept {

    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle dividing to result
  constexpr vec3_t &operator/=(const T &rhs) noexcept {

    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle changing value to vector
  constexpr vec3_t &operator=(const T &rhs) noexcept {

    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle arrays
  constexpr T &operator[](std::size_t index) noexcept {

    // (datatype)pointer + index
    return *((T *)this + index);
  }
};

template <class T> struct vec2_t {

  T x, y;

  static_assert(std::is_arithmetic<T>::value, "T must be numeric");

  constexpr vec2_t() : x(0), y(0) {}

  constexpr vec2_t(const T _x, const T _y) : x(_x), y(_y) {}

  constexpr vec2_t(const vec2_t &vec) : x(vec.x), y(vec.y) {}

  constexpr bool is_valid() noexcept {

    // return validity state
    return std::isfinite(this->x) && std::isfinite(this->y);
  }

  constexpr T size() noexcept {

    // return size
    return sizeof(vec2_t(this->x, this->y));
  }

  constexpr T length() noexcept {

    // return length
    return sqrt((this->x * this->x) + (this->y * this->y));
  }

  constexpr T distance(const vec2_t &rhs) noexcept {

    // calculate distance, returns base vector - argument vector to length
    return (*this - rhs).length();
  }

  constexpr vec2_t normalize() noexcept {

    // we return a new vector with our new values
    return vec2_t(x / length(), y / length());
  }

  constexpr T dot(const vec2_t &rhs) noexcept {

    // calculate dot product of two vectors
    return (this->x * rhs.x) + (this->y * rhs.y);
  }

  constexpr bool empty() noexcept {

    // return as empty
    return this->x == 0 && this->y == 0;
  }

  constexpr bool equal(const vec2_t &rhs) noexcept {

    // return equality state
    return this->x == rhs.x && this->y == rhs.y;
  }

  // handle adding
  constexpr vec2_t operator+(const vec2_t &rhs) noexcept {

    // return value handled as array of results into a new vector
    return vec2_t(this->x + rhs.x, this->y + rhs.y);
  }

  // handle substracting
  constexpr vec2_t operator-(const vec2_t &rhs) noexcept {

    // return value handled as array of results into a new vector
    return vec2_t(this->x - rhs.x, this->y - rhs.y);
  }

  // handle multiplying
  constexpr vec2_t operator*(const T &rhs) noexcept {

    // we return a new vector with our new values
    return vec2_t(this->x * rhs.x, this->y * rhs.y);
  }

  // handle divisions
  constexpr vec2_t operator/(const T &rhs) noexcept {

    // we return a new vector with our new values
    return vec2_t(this->x / rhs.x, this->y / rhs.y);
  }

  // handle adding to result
  const vec2_t &operator+=(const vec2_t &rhs) noexcept {

    this->x += rhs.x;
    this->y += rhs.y;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle substracting to result
  constexpr vec2_t &operator-=(const vec2_t &rhs) noexcept {

    this->x -= rhs.x;
    this->y -= rhs.y;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle multiplying to result
  constexpr vec2_t &operator*=(const T &rhs) noexcept {

    this->x *= rhs.x;
    this->y *= rhs.y;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle dividing to result
  constexpr vec2_t &operator/=(const T &rhs) noexcept {

    this->x /= rhs.x;
    this->y /= rhs.y;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle changing value to vector
  constexpr vec2_t &operator=(const T &rhs) noexcept {

    this->x = rhs.x;
    this->y = rhs.y;

    // return value handled as array of results into a new vector
    return *this;
  }

  // handle arrays
  constexpr T &operator[](std::size_t index) noexcept {

    // (datatype)pointer + index
    return *((T *)this + index);
  }
};
} // namespace util
