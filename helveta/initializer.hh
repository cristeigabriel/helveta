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
#define offset( t, base, off ) \
        *reinterpret_cast< t * >( std::uintptr_t( base ) + off )

// STL
#include <cinttypes>
#include <algorithm>
#include <iostream>
#include <optional>
#include <math.h>
#include <cstdio>
#include <array>

// library files
#include "math/vector.hh"
#include "math/matrix.hh"
#include "misc/hasher.hh"
#include "memory/memory.hh"
