#pragma once
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "hittableList.h"

class translate : public hittable
{
public:
	translate() = default;
	translate(shared_ptr<hittable> p, const vec3& displacement)
		: ptr(p), mOffset(displacement) {}

	virtual bool hit(const ray& r, double t0, double t1, hitRecord& rec) const override;
	virtual bool boundingBox(double t0, double t1, aabb& outputBox) const override;
public:
	shared_ptr<hittable> ptr;
	vec3 mOffset;
};

bool translate::hit(const ray& r, double t0, double t1, hitRecord& rec) const
{
	// move ray instead of hittable object
	ray movedR(r.origin() - mOffset, r.direction(), r.time());
	if (!ptr->hit(movedR, t0, t1, rec))
		return false;

	rec.p += mOffset;
	rec.setFaceNormal(movedR, rec.normal);
	return true;
}

bool translate::boundingBox(double t0, double t1, aabb& outputBox) const
{
	if (!ptr->boundingBox(t0, t1, outputBox))
		return false;

	outputBox = aabb(
		outputBox.min() + mOffset,
		outputBox.max() + mOffset
	);

	return true;
}
#endif // !TRANSLATE_H

