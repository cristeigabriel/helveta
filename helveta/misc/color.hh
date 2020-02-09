/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects
*written by: Cristei Gabriel-Marian
*file written by: Cristei Gabriel, base by DucaRii, inspired from CSGO Modest.
*licensing: MIT License

*file description: color class
*/

namespace helveta {

namespace color {

template <class T>
struct color_t {

  T r, g, b, a;

  static_assert(std::is_arithmetic<T>::value, "T must be numeric");

  constexpr color_t() = default;
  constexpr color_t(const T _r, const T _g, const T _b)
      : r(_r), g(_g), b(_b), a(255) {}
  constexpr color_t(const T _r, const T _g, const T _b, const T _a)
      : r(_r), g(_g), b(_b), a(_a) {}
  constexpr color_t(const color_t &col, const T _a)
      : r(col->r), g(col->g), b(col->b), a(_a) {}

  float hue() const noexcept {

    // handle values
    float r = this->r / 255.f;
    float g = this->g / 255.f;
    float b = this->b / 255.f;

    // handle min&max
    float max = std::max(r, std::max(g, b));
    float min = std::min(r, std::min(g, b));

    if (min == max) {

      // return null value
      return 0.f;
    }

    // calculate delta
    float delta = max - min;

    // initialize hue
    float hue = 0.f;

    if (max == r)
      hue = (g - b) / delta;
    else if (max == g)
      hue = 2.f + (b - r) / delta;
    else
      hue = 4.f + (r - g) / delta;

    // mult. hue by 60
    hue *= 60.f;

    // loop it
    if (hue < 0.f) hue += 360.f;

    // return value
    return hue / 360.f;
  }

  float saturation() const noexcept {

    // handle values
    float r = this->r / 255.f;
    float g = this->g / 255.f;
    float b = this->b / 255.f;

    // handle min&max
    float max = std::max(r, std::max(g, b));
    float min = std::min(r, std::min(g, b));

    // calculate delta
    float delta = max - min;

    // return delta if max = 0
    if (max == 0.f) return delta;

    // return value
    return delta / max;
  }

  float brightness() const noexcept {

    // handle values
    float r = this->r / 255.0f;
    float g = this->g / 255.0f;
    float b = this->b / 255.0f;

    // return value
    return std::max(r, std::max(g, b));
  }

  static color_t from_hsb(float hue, float saturation, float brightness) {

    // handle values
    float h = hue == 1.0f ? 0 : hue * 6.0f;
    float f = h - (int)h;
    float p = brightness * (1.0f - saturation);
    float q = brightness * (1.0f - saturation * f);
    float t = brightness * (1.0f - (saturation * (1.0f - f)));

    if (h < 1) {

      return color_t(static_cast<unsigned char>(brightness * 255),
                     static_cast<unsigned char>(t * 255),
                     static_cast<unsigned char>(p * 255));
    }
    if (h < 2) {

      return color_t(static_cast<unsigned char>(q * 255),
                     static_cast<unsigned char>(brightness * 255),
                     static_cast<unsigned char>(p * 255));
    }
    if (h < 3) {

      return color_t(static_cast<unsigned char>(p * 255),
                     static_cast<unsigned char>(brightness * 255),
                     static_cast<unsigned char>(t * 255));
    }
    if (h < 4) {

      return color_t(static_cast<unsigned char>(p * 255),
                     static_cast<unsigned char>(q * 255),
                     static_cast<unsigned char>(brightness * 255));
    }
    if (h < 5) {

      return color_t(static_cast<unsigned char>(t * 255),
                     static_cast<unsigned char>(p * 255),
                     static_cast<unsigned char>(brightness * 255));
    }

    return color_t(static_cast<unsigned char>(brightness * 255),
                   static_cast<unsigned char>(p * 255),
                   static_cast<unsigned char>(q * 255));
  }

  // handle adding
  constexpr color_t &operator+(const color_t &rhs) noexcept {

    // return value handled as array of results into a new color
    return color_t(this->r + rhs.r, this->g + rhs.g, this->b + rhs.b,
                   this->a + rhs.a);
  }

  // handle substracting
  constexpr color_t &operator-(const color_t &rhs) noexcept {

    // return value handled as array of results into a new color
    return color_t(this->r - rhs.r, this->g - rhs.g, this->b - rhs.b,
                   this->a - rhs.a);
  }

  // handle multiplying
  constexpr color_t &operator*(const T &rhs) noexcept {

    // we return a new color with our new values
    return color_t(this->r * rhs.r, this->g * rhs.g, this->b * rhs.b,
                   this->a * rhs.a);
  }

  // handle divisions
  constexpr color_t &operator/(const T &rhs) noexcept {

    // we return a new color with our new values
    return color_t(this->r / rhs.r, this->g / rhs.g, this->b / rhs.b,
                   this->a / rhs.a);
  }

  // handle adding to result
  const color_t &operator+=(const color_t &rhs) noexcept {

    this->r += rhs.r;
    this->g += rhs.g;
    this->b += rhs.b;
    this->a += rhs.a;

    // return value handled as array of results into a new color
    return *this;
  }

  // handle substracting to result
  constexpr color_t &operator-=(const color_t &rhs) noexcept {

    this->r -= rhs.r;
    this->g -= rhs.g;
    this->b -= rhs.b;
    this->a -= rhs.a;

    // return value handled as array of results into a new color
    return *this;
  }

  // handle multiplying to result
  constexpr color_t &operator*=(const T &rhs) noexcept {

    this->r *= rhs.r;
    this->g *= rhs.g;
    this->b *= rhs.b;
    this->a *= rhs.a;

    // return value handled as array of results into a new color
    return *this;
  }

  // handle dividing to result
  constexpr color_t &operator/=(const T &rhs) noexcept {

    this->r /= rhs.r;
    this->g /= rhs.g;
    this->b /= rhs.b;
    this->a /= rhs.a;

    // return value handled as array of results into a new color
    return *this;
  }

  // handle assigning
  constexpr color_t &operator=(const color_t &rhs) {

    // assign current rgba to rhs rgba
    this->r = rhs.r;
    this->g = rhs.g;
    this->b = rhs.b;
    this->a = rhs.a;

    // return value handled as array of results into a new color
    return *this;
  }

  // handle arrays
  constexpr T &operator[](std::size_t index) {

#ifdef DEBUG
    assert(index <= 3, "INDEX out of range");
#endif
    if (index > 3) { throw std::exception("INDEX out of range"); }
  	
    // (datatype)pointer + index
    return *(reinterpret_cast<T *>(this) + index);
  }
};
} // namespace color
} // namespace helveta