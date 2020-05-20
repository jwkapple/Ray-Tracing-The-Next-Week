#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture
{
	virtual color value(double u, double v, const point3& p) const;
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
#endif // !TEXTURE_H
