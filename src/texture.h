#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture
{
public:
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solidColor : public texture
{
public:
	solidColor() = default;
	solidColor(color c) : mColorValue(c) {}
	solidColor(double r, double g, double b) : solidColor(color(r,g,b)) {}

	virtual color value(double u, double v, const point3& p) const override
	{
		return mColorValue;
	}

private:
	color mColorValue;
};

class checkerTexture : public texture
{
public:
	checkerTexture() = default;
	checkerTexture(shared_ptr<texture> t0, shared_ptr<texture> t1)
		: odd(t0), even(t1) {}

	virtual color value(double u, double v, const point3& p) const override
	{
		auto sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);

		else
			return even->value(u, v, p);
	}

public:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
};
#endif // !TEXTURE_H
