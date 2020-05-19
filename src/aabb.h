#pragma once

#include "rtweekend.h"

class aabb
{
public:
	aabb() = default;
	aabb(const point3& a, const point3& b) : _min(a), _max(b) {}

	point3 min() const { return _min; }
	point3 max() const { return _max; }

	bool hit(const ray& r, double tMin, double tMax) const
	{
		for (int a = 0; a < 3; a++)
		{
			auto invD = 1.0f / r.direction()[a];
			auto t0 = (_min[a] - r.origin()[a]) * invD;
			auto t1 = (_max[a] - r.origin()[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);

			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;

			if (tMax <= tMin)
				return false;
		}
		return true;
	}


public:
	point3 _min;
	point3 _max;
};

aabb surroundingBox(aabb box0, aabb box1)
{
	auto t0 = point3(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	auto t1 = point3(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));

	return aabb(t0, t1);
}
