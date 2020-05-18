#pragma once

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() = default; // Option::const vec3 &origin, const vec3 &direction
	ray(const vec3 &origin, const vec3 &direction)
		: mOrigin(origin), mDirection(direction)
	{}

	vec3 origin() const { return mOrigin; }
	vec3 direction() const { return mDirection; }

	vec3 at(double t) const
	{
		return mOrigin + t * mDirection;
	}
public:
	vec3 mOrigin;
	vec3 mDirection;
};

#endif