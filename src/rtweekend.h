#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793285;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180;
}

inline double randomDouble()
{
	// Returns a random real in (1,0)
	return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max)
{
	// Returns a random real in [min,max)
	return min + (max - min) * randomDouble();
}

inline double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif // !RTWEEKEND_H
