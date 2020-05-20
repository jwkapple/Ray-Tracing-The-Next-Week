#pragma once
#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "hittableList.h"
#include "rtweekend.h"

class bvhNode : public hittable
{
public:
	bvhNode(hittableList& list, double time0, double time1)
		: bvhNode(list.objects, 0, list.objects.size(), time0, time1) {}

	bvhNode(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1);


	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;
	virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override;
public:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb box;
};

bool BoxCompare(const shared_ptr<hittable>& box0, const shared_ptr<hittable>& box1, int axis)
{
	aabb boxA;
	aabb boxB;

	if (box0->boundingBox(0, 0, boxA) || box1->boundingBox(0, 0, boxB))
		std::cerr << "No Bounding box in bvhNode constructor. \n";

	return boxA.min().e[axis] < boxB.min().e[axis];
}

bool boxXCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return BoxCompare(a, b, 0);
}

bool boxYCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return BoxCompare(a, b, 1);
}

bool boxZCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return BoxCompare(a, b, 2);
}

bvhNode::bvhNode(
	std::vector<shared_ptr<hittable>>& objects,
	size_t start, size_t end, double time0, double time1)
{
	int axis = static_cast<int>(randomDouble(0, 3));
	auto comparator = (axis == 0) ? boxXCompare
		: (axis == 1) ? boxYCompare
		: boxZCompare;

	size_t object_span = end - start;

	if (object_span == 1)
	{
		left = right = objects[start];
	}
	else if (object_span == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin(), objects.begin() + end, comparator);

		auto mid = (start + end) / 2;
		left = std::make_shared<bvhNode>(objects, start, mid, time0, time1);
		right = std::make_shared<bvhNode>(objects, mid, end, time0, time1);
	}

	aabb boxLeft, boxRight;

	if (!left->boundingBox(time0, time1, boxLeft) ||
		!right->boundingBox(time0, time1, boxRight))
	{
		std::cerr << "No bounding box in bvhNode constructor. \n";
	}

	box = surroundingBox(boxLeft, boxRight);
}

bool bvhNode::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const
{
	if (!box.hit(r, tMin, tMax))
		return false;

	bool hitLeft = left->hit(r, tMin, tMax, rec);
	bool hitRight = right->hit(r, tMin, hitLeft ? rec.t : tMax, rec);

	return hitLeft || hitRight;
}

bool bvhNode::boundingBox(double time0, double time1, aabb& outputBox) const
{
	outputBox = box;
	return true;
}
#endif // !BVH_H
