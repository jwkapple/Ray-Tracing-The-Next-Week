#pragma once
#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

inline double trilinear_interp(vec3 c[2][2][2], double u, double v, double w)
{
	auto uu = u * u*(3 - 2 * u);
	auto vv = v * v*(3 - 2 * v);
	auto ww = w * w*(3 - 2 * w);
	auto accum = 0.0;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				vec3 weight_v(u - i, v - j, w - k);
				accum += (i*uu + (1 - i)*(1 - uu))
					* (j*vv + (1 - j)*(1 - vv))
					* (k*ww + (1 - k)*(1 - ww))
					* dot(c[i][j][k], weight_v);
			}


	return accum;
}

class perlin
{
public:
	perlin()
	{
		ranvec = new vec3[mPointCount];

		for (int i = 0; i < mPointCount; ++i) {
			ranvec[i] = unit_vector(vec3::random(-1, 1));
		}

		mPermX = perlinGeneratePerm();
		mPermY = perlinGeneratePerm();
		mPermZ = perlinGeneratePerm();
	}

	~perlin()
	{
		delete[] ranvec;
		delete[] mPermX;
		delete[] mPermY;
		delete[] mPermZ;
	}

	double noise(const point3& p) const
	{
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());

		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranvec[
						mPermX[(i + di) & 255] ^
						mPermY[(j + dj) & 255] ^
						mPermZ[(k + dk) & 255]
					];

		return trilinear_interp(c, u, v, w);
	}

	double turb(const point3& p, int depth = 7) const
	{
		auto accum = 0.0;
		auto tmp = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(tmp);
			weight *= 0.5;
			tmp *= 2;
		}

		return fabs(accum);
	}

private:
	static const int mPointCount = 256;
	vec3* ranvec;
	int* mPermX;
	int* mPermY;
	int* mPermZ;

	inline static int* perlinGeneratePerm()
	{
		// Prepare array before permute and returns array
		auto p = new int[mPointCount];

		for (int i = 0; i < mPointCount; i++)
			p[i] = i;

		permute(p, mPointCount);

		return p;
	}

	inline static void permute(int* p, int n)
	{
		// Swap random two positions
		for (int i = n - 1; i > 0; i--)
		{
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}
};
#endif // !PERLIN_H
