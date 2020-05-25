#pragma once
#ifndef ROTATEY_H
#define ROTATEY_H

#include "hittableList.h"

class rotateY : public hittable
{
public:
	rotateY() = default;
	rotateY(shared_ptr<hittable> p, double angle);

	virtual bool hit(const ray& r, double t0, double t1, hitRecord& rec) const override;
	virtual bool boundingBox(double t0, double t1, aabb& outputBox) const override;
public:
	shared_ptr<hittable> ptr;
	double sinTheta;
	double cosTheta;
	bool hasBox;
	aabb bbox;
};

rotateY::rotateY(shared_ptr<hittable> p, double angle) : ptr(p)
{
	auto radians = degrees_to_radians(angle);
	sinTheta = sin(radians);
	cosTheta = cos(radians);
	hasBox = ptr->boundingBox(0, 1, bbox);

	point3 min(infinity);
	point3 max(-infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i * bbox.max().x() + (1 - i)*bbox.min().x();
				auto y = j * bbox.max().y() + (1 - j)*bbox.min().y();
				auto z = k * bbox.max().z() + (1 - k)*bbox.min().z();

				auto newx = cosTheta * x + sinTheta * z;
				auto newz = -sinTheta * x + cosTheta * z;

				vec3 tester(newx, y, newz);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotateY::hit(const ray& r, double t0, double t1, hitRecord& rec) const
{
	auto origin = r.origin();
	auto direction = r.direction();

	origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
	origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];
	
	direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
	direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];

	ray rotatedR(origin, direction, r.time());

	if (!ptr->hit(rotatedR, t0, t1, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
	p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];

	normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
	normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

	rec.p = p;
	rec.setFaceNormal(rotatedR, normal);

	return true;
}

bool rotateY::boundingBox(double t0, double t1, aabb& outputBox) const
{
	if (!ptr->boundingBox(t0, t1, outputBox))
		return false;

	outputBox = bbox;

	return true;
}
#endif // !ROTATEY_H
