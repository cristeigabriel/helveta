/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian [& Octav Adrian] 
*licensing: MIT License

*file description: matrix class, base code credits to Octav Adrian
*/

namespace util {

struct matrix4x4_t {

  // elements handler
  std::array<float, 16> elements;

  constexpr matrix4x4_t() : elements({0.f}) {}
  constexpr matrix4x4_t(const float &diag) : elements({0.f}) {

    // assign values at array index
    this->elements.at(0)  = diag;
    this->elements.at(5)  = diag;
    this->elements.at(10) = diag;
    this->elements.at(15) = diag;
  }

  constexpr matrix4x4_t identity() noexcept {

    // return identity
    return matrix4x4_t(1.0f);
  }

  constexpr matrix4x4_t ortho(const float &left, const float &right,
                              const float &top, const float &bottom,
                              const float &near, const float &far) noexcept {
    // class derrived object
    auto res = matrix4x4_t();

    res.elements.at(0)  = 2.f / (right - left);
    res.elements.at(5)  = 2.f / (top - bottom);
    res.elements.at(10) = 2.f / (far - near);
    res.elements.at(15) = 1;

    res.elements.at(3)  = (right + left) / (right - left);
    res.elements.at(7)  = (bottom + top) / (bottom - top);
    res.elements.at(11) = (far + near) / (far - near);

    // return value after calculations
    return res;
  }

  matrix4x4_t perspective(const float &fov, const float &ratio,
                          const float &near, const float &far) noexcept {

    // class derrived object
    matrix4x4_t res = matrix4x4_t();

    float q = 1.0f / std::tanf(DEG2RAD(0.5f * fov));
    float a = q / ratio;
    float b = (near + far) / (near - far);
    float c = (2.f * near * far) / (near - far);

    res.elements.at(0)  = a;
    res.elements.at(5)  = q;
    res.elements.at(10) = b;
    res.elements.at(11) = -1.f;
    res.elements.at(14) = c;

    // return value after calculations
    return res;
  }

  matrix4x4_t rotation(const float &              angle,
                       const util::vec3_t<float> &axis) noexcept {

    // class derrived object
    matrix4x4_t a   = matrix4x4_t(1.f);
    float       ang = DEG2RAD(angle);

    float cos = std::cos(ang);
    float sin = std::sin(ang);
    float omc = 1.f - cos;

    a.elements.at(0) = axis.x * omc + cos;
    a.elements.at(1) = axis.y * axis.x * omc + axis.z * sin;
    a.elements.at(2) = axis.z * axis.x * omc - axis.z * sin;

    a.elements.at(4) = axis.x * axis.y * omc - axis.z * sin;
    a.elements.at(5) = axis.y * omc + cos;
    a.elements.at(6) = axis.y * axis.z * omc + axis.x * sin;

    a.elements.at(8)  = axis.x * axis.z * omc + axis.y * sin;
    a.elements.at(9)  = axis.y * axis.z * omc - axis.x * sin;
    a.elements.at(10) = axis.y * omc + cos;

    // return value after calculations
    return a;
  }

  constexpr matrix4x4_t scale(const util::vec3_t<float> &sca) noexcept {

    // class derrived object
    matrix4x4_t a = matrix4x4_t(1.f);

    a.elements.at(0)  = sca.x;
    a.elements.at(5)  = sca.y;
    a.elements.at(10) = sca.z;

    // return value after calculations
    return a;
  }

  // handle multiplying
  constexpr matrix4x4_t &operator*(const matrix4x4_t &a) noexcept {

    // loops
    for (int y = 0; y < 4; y++) {

      for (int x = 0; x < 4; x++) {

        float sum = 0.f;

        for (int e = 0; e < 4; e++) {

          // add arr to sum
          sum += this->elements.at(x + e * 4) * a.elements.at(e + y * 4);
        }

        // change value of array index
        this->elements[x + y * 4] = sum;
      }
    }

    // return value after calculations
    return *this;
  }

  // handle multiplying from curr value
  constexpr matrix4x4_t &operator*=(const matrix4x4_t &a) noexcept {

    // loops
    for (int y = 0; y < 4; y++) {

      for (int x = 0; x < 4; x++) {

        float sum = 0.f;

        for (int e = 0; e < 4; e++) {

          // add arr to sum
          sum += this->elements.at(x + e * 4) * a.elements.at(e + y * 4);
        }

        // change value of array index
        this->elements[x + y * 4] = sum;
      }
    }

    // return value after calculations
    return *this;
  }
};

} // namespace util