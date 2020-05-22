#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#define STB_IMAGE_IMPLEMENTATION

#include "rtweekend.h"
#include "perlin.h"
#include "stb_image.h"

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

class noiseTexture : public texture
{
public:
	noiseTexture() = default;
	noiseTexture(double scale) : mScale(scale) {}
	virtual color value(double u, double v, const vec3& p) const override
	{
		return color(1) * 0.5 * (1.0 + sin(mScale * p.z() + 10 * mNoise.turb(p)));
	//	return color(1) * mNoise.turb(p);
	}
public:
	perlin mNoise;
	double mScale;
};

class imageTexture : public texture
{
public:
	const static int mBytesPerPixel = 3;

	imageTexture()
		:mData(nullptr), mWidth(0), mHeight(0), mBytesPerScanline(0) {}

	imageTexture(const char* filename)
	{
		auto componentsPerPixel = mBytesPerPixel;

		mData = stbi_load(filename, &mWidth, &mHeight, &componentsPerPixel, componentsPerPixel);

		if (!mData)
		{
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'. \n";
			mWidth = mHeight = 0;
		}

		mBytesPerScanline = mBytesPerPixel * mWidth;
	}

	~imageTexture()
	{
		delete mData;
	}

	virtual color value(double u, double v, const vec3& p) const override
	{
		// If we have no texture data, then return solid cyan as a debugging aid.
		if (mData == nullptr)
			return color(0, 1, 1);

		// Clamp input texture coordinates to [0,1] x [1,0]
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

		auto i = static_cast<int>(u * mWidth); // vec3 for width
		auto j = static_cast<int>(v * mHeight); // vec3 fro height

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= mWidth) i = mWidth - 1;
		if (j >= mHeight) j = mHeight - 1;

		const auto colorScale = 1.0 / 255.0;
		auto pixel = mData + j * mBytesPerScanline + i * mBytesPerPixel;

		return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
private:
	unsigned char* mData;
	int mWidth, mHeight;
	int mBytesPerScanline;
};
#endif // !TEXTURE_H
