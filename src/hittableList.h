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
	virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override;

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

bool hittableList::boundingBox(double time0, double time1, aabb& outputBox) const
{
	if (objects.empty()) return false;

	aabb tmpBox;
	bool firstBox = true;

	for (auto object : objects)
	{
		if (!object->boundingBox(time0, time1, tmpBox)) return false;
		outputBox = firstBox ? tmpBox : surroundingBox(tmpBox, outputBox);
		firstBox = false;
	}

	return true;
}

#endif // !HITTABLE_LIST_H
