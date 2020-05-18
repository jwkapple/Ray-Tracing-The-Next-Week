#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittableList : public hittable
{
public: 
	hittableList() = default;
	hittableList(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

public:
	std::vector<shared_ptr<hittable>> objects;
};

bool hittableList::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const // Configure closest hitPoint
{
	hitRecord tmpRec;
	bool hitAnything = false;
	double closestSoFar = tMax;

	for (const auto& object : objects)
	{
		if (object->hit(r, tMin, closestSoFar, tmpRec))
		{
			hitAnything = true;
			closestSoFar = tmpRec.t;
			rec = tmpRec;
		}
	}

	return hitAnything;
}

#endif // !HITTABLE_LIST_H
