#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
public:
	camera(
		point3 lookFrom, point3 lookAt, vec3 vUp,
		double vFov, // vertical field-of-view in degrees
		double aspectRatio,
		double time0,
		double time1
	)
	{
		mOrigin = lookFrom;


		auto theta = degrees_to_radians(vFov);
		auto half_height = tan(theta / 2);
		auto half_width = aspectRatio * half_height;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(w, vUp));
		v = cross(w, u);

		mLowerLeftCorner = point3(-half_width, -half_height, -1.0);

		mHorizontal = vec3(2 * half_width, 0.0, 0.0);
		mVertical = vec3(0.0, 2 * half_height, 0.0);

		mTime0 = time0;
		mTime1 = time1;
	}

	ray get_ray(double u, double v) const
	{
		return ray(mOrigin, mLowerLeftCorner + u * mHorizontal + v * mVertical - mOrigin, randomDouble(mTime0, mTime1));
	}

private:
	point3 mOrigin;
	point3 mLowerLeftCorner;
	vec3 mVertical;
	vec3 mHorizontal;
	vec3 u, v, w;

	double mTime0, mTime1;
};
#endif // !CAMERA_H
