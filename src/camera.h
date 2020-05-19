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
		auto height = 2* tan(theta / 2);
		auto width = aspectRatio * height;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);

		mHorizontal = width * u;
		mVertical = height * v;
		mLowerLeftCorner = mOrigin - mHorizontal / 2 - mVertical / 2 - w;

		mTime0 = time0;
		mTime1 = time1;
	}

	ray get_ray(double s, double t) const
	{

		return ray(mOrigin, mLowerLeftCorner + s * mHorizontal + t * mVertical - mOrigin, randomDouble(mTime0, mTime1));
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
