#pragma once

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() = default; // Option::const vec3 &origin, const vec3 &direction
	ray(const vec3 &origin, const vec3 &direction, double time = 0.0)
		: mOrigin(origin), mDirection(direction), mTime(time)
	{}

	vec3 origin() const { return mOrigin; }
	vec3 direction() const { return mDirection; }
	double time() const { return mTime; }

	vec3 at(double t) const
	{
		return mOrigin + t * mDirection;
	}
public:
	vec3 mOrigin;
	vec3 mDirection;
	double mTime;
};

#endif