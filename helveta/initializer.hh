/*
 *project name: helveta
 *purpose: multi-purpose shared library for windows, made to be submodule for projects
 *written by: Cristei Gabriel-Marian
 *licensing: MIT License
 */

// math basics

// std::atan(1) * 4
constexpr float PI = 3.141593f;
#define DEG2RAD(DEG) ((float)DEG * (180 / PI))
#define RAD2DEG(RAD) ((float)RAD * (PI / 180))


// STL
#include <algorithm>
#include <iostream>
#include <math.h>
#include <array>

// library files
#include "math/vector.hh"
#include "math/matrix.hh"
#include "misc/hasher.hh"