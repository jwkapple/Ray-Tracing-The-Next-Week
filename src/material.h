#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "ray.h"
#include "hittable.h"

class material
{
public:
	virtual bool scatter
	(
		const ray& r_in, const hitRecord& rec, color& attenuation, ray& scatterd
	) const = 0;
};

double schlick(double cosine, double ref_idx) {
	auto r0 = (1-ref_idx) / (1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1 - cosine),5);
}

class lambertian : public material
{
public:
	lambertian(const color& a) : mAlbedo(a) { }

	virtual bool scatter
	(
		const ray& r_in, const hitRecord& rec, color& attenuation, ray& scatterd
	) const override {
		vec3 scatterDirection = rec.normal + random_unit_vector();
		scatterd = ray(rec.p, scatterDirection);
		attenuation = mAlbedo;
		return true;
	}
	
private:
	color mAlbedo;
};

class metal : public material
{
public:
	metal() = default;
	metal(const color& a) : mAlbedo(a) {}

	virtual bool scatter
	(
		const ray& ray_in, const hitRecord& rec, color& attenuation, ray& scattered
	) const override
	{
		vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
		attenuation = mAlbedo;
		scattered = ray(rec.p, reflected);
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color mAlbedo;
};

class dielectric : public material {

	public:
		dielectric(double ri) : ref_idx(ri) { }
		virtual bool scatter(
				const ray& r_in, const hitRecord& rec, color& attenuation, ray& scattered
				) const override
		{
			attenuation = color(1.0);
			double etai_over_etat;
			if (rec.frontFace) {
				etai_over_etat = 1.0 / ref_idx;
			}
			else
			{
				etai_over_etat = ref_idx;
			}

			vec3 unitDirection = unit_vector(r_in.direction());
			double cos_theta = fmin(dot(-unitDirection, rec.normal), 1.0);
			double sin_theta = sqrt(1 - cos_theta * cos_theta);
			if(etai_over_etat * sin_theta > 1.0)
			{
				vec3 reflected = reflect(unitDirection, rec.normal);
				scattered = ray(rec.p, reflected);
				return true;
			}

			double reflect_prob = schlick(cos_theta, etai_over_etat);
			if (randomDouble() < reflect_prob) {
				vec3 reflected = reflect(unitDirection, rec.normal);
				scattered = ray(rec.p, reflected);
				return true;
			}

			vec3 refracted = refract(unitDirection, rec.normal, etai_over_etat);
			scattered = ray(rec.p, refracted);
			return true;
		}
	public:
	double ref_idx;
};
#endif // !MATERIAL_H

