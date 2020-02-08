/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects 
*written by: Cristei Gabriel-Marian

*licensing: MIT License
*/

// math basics
constexpr float PI = 3.141593f;
#define DEG2RAD(DEG) ((float)(DEG) * (180 / PI))
#define RAD2DEG(RAD) ((float)(RAD) * (PI / 180))

// memory
#define offset(t, base, off) *reinterpret_cast<t *>(std::uintptr_t(base) + off)

// STL
#include <algorithm>
#include <array>
#include <cinttypes>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <optional>

// library files
#include "math/matrix.hh"
#include "math/vector.hh"
#include "memory/memory.hh"
#include "misc/hasher.hh"
