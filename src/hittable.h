#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

class material;

struct hitRecord
{
	point3 p;
	vec3 normal;
	shared_ptr<material> matPtr;
	bool frontFace{false};
	double t;
	double u;
	double v;

	inline void setFaceNormal(const ray& r, const vec3& outwardNormal)
	{
		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const = 0;
	// tMin & tMax set for limitation of ray distance

	virtual bool boundingBox(double time0, double time1, aabb& outputBox) const = 0;
};

#endif // !HITTABLE_H

