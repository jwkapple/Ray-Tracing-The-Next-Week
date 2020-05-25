#pragma once
#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"
#include "aarect.h"
#include "hittableList.h"

class box : public hittable
{
public:
	box() = default;
	box(const point3& p0, const point3& p1, shared_ptr<material> mat);

	virtual bool hit(const ray& r, double t0, double t1, hitRecord& rec) const override;

	virtual bool boundingBox(double t0, double t1, aabb& outputBox) const override
	{
		outputBox = aabb(mBoxMin, mBoxMax);
		return true;
	}
public:
	point3 mBoxMin;
	point3 mBoxMax;
	hittableList sides;
};

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr)
{
	mBoxMin = p0;
	mBoxMax = p1;

	sides.add(make_shared<xyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(make_shared<xyRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

	sides.add(make_shared<xzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(make_shared<xzRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

	sides.add(make_shared<yzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(make_shared<yzRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t0, double t1, hitRecord& rec) const
{
	return sides.hit(r, t0, t1, rec);
}
#endif // !BOX_H
