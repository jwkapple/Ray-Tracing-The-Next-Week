#pragma once
#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H

#include "hittable.h"
#include "vec3.h"

using std::shared_ptr;

class movingSphere : public hittable
{
public:
	movingSphere() = default;
	movingSphere(
		point3 cen0, point3 cen1, double time0, double time1, double r, std::shared_ptr<material> m)
		: mCenter0(cen0), mCenter1(cen1), mTime0(time0), mTime1(time1), mRadius(r), mMatPtr(m)
		{};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;
	
	point3 center(double time) const;
public:
	point3 mCenter0, mCenter1;
	double mTime0, mTime1;
	double mRadius;
	std::shared_ptr<material> mMatPtr;	
};

point3 movingSphere::center(double time) const
{
	return mCenter0 + ((time - mTime0) / (mTime1 - mTime0)) * (mCenter1 - mCenter0);
}

bool movingSphere::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	vec3 oc = r.origin() - center(r.time());
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
			vec3 outwardNormal = (rec.p - center(r.time())) / mRadius;
			rec.setFaceNormal(r, outwardNormal);
			return true;
		}

		tmp = (-half_b + root) / a;
		if (tmp < tMax && tmp > tMin)
		{
			rec.t = tmp;
			rec.p = r.at(rec.t);
			rec.matPtr = mMatPtr;
			vec3 ourwardNormal = (rec.p - center(r.time())) / mRadius;
			rec.setFaceNormal(r, ourwardNormal);
			return true;
		}
	}

	return false;
}

#endif // !MOVINGSPHERE_H

