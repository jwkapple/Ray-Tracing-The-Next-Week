#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere() = default;
	sphere(point3 cen, double r, shared_ptr<material> m) : mCenter(cen), mRadius(r), mMatPtr(m) {};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

public:
	point3 mCenter;
	double mRadius;
	shared_ptr<material> mMatPtr;
};

bool sphere::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	vec3 oc = r.origin() - mCenter;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - mRadius * mRadius;
	auto discreminant = half_b * half_b - a * c;

	if (discreminant > 0)
	{
		auto root = sqrt(discreminant);
		auto tmp = (-half_b - root) / a;

		if (tmp < tMax && tmp > tMin)
		{
			rec.t = tmp;
			rec.p = r.at(rec.t);
			rec.matPtr = mMatPtr;
			vec3 outwardNormal = (rec.p - mCenter) / mRadius;
			rec.setFaceNormal(r, outwardNormal);
			return true;
		} // Send hit record to Ray with hitRecord

		tmp = (-half_b + root) / a;
		if (tmp < tMax && tmp > tMin)
		{
			rec.t = tmp;
			rec.p = r.at(rec.t);
			rec.matPtr = mMatPtr;
			vec3 outwardNormal = (rec.p - mCenter) / mRadius;
			rec.setFaceNormal(r, outwardNormal);
			return true;
		}
	}
	return false;
}

#endif // !SPHERE_H
