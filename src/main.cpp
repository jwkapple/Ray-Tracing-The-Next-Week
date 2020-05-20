
#include "rtweekend.h"

#include "bvh.h"
#include "material.h"
#include "hittableList.h"
#include "sphere.h"
#include "movingSphere.h"
#include "color.h"
#include "camera.h"
#include <iostream>



vec3 rayColor(const ray &r, const hittable& world, int depth)
{
	hitRecord rec;
	
	// If we've exceeded the ray bounce limit, no mor light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// Some of the reflected rays hit the object they are reflecting off of not at exactly t = 0 
	// So we need to ignore hits very near zero

	if (world.hit(r, 0.001, infinity, rec))
	{
		color attenuation;
		ray scattered;
		if (rec.matPtr->scatter(r, rec, attenuation, scattered))
			return attenuation * rayColor(scattered, world, depth - 1);
		return color(0);
	}


	vec3 unitDirection = unit_vector(r.direction());
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittableList randomScene()
{
	hittableList world;

	for (int a = -4; a < 4; a++) {
		for (int b = -4; b < 4; b++) {
			auto choose_mat = randomDouble();
			point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, randomDouble(0, .5), 0);
					auto ranTime = randomDouble(0,1);
					world.add(make_shared<movingSphere>(
						center, center2, 0.0, 1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					sphere_material = make_shared<metal>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	
		world.add(make_shared<sphere>(
			point3(0, -1000, 0), 1000, make_shared<lambertian>(color(0.5, 0.5, 0.5))));
	
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(point3(-4, 1, 0), 1.0, make_shared<lambertian>(color(.4, .2, .1))));

	world.add(
		make_shared<sphere>(point3(4, 1, 0), 1.0, make_shared<metal>(color(.7, .6, .5))));

		return world;
}

int main()
{
	const auto aspectRatio = 16.0 / 9.0;
	const int imageWidth = 384;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samples_per_pixel = 100;
	const int maxDepth = 10;
	const vec3 vUp = vec3(0, 1, 0);
	const double vFov = 90;
	const double time0 = 0.0;
	const double time1 = 1.0;
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	hittableList world = randomScene();

	auto R = cos(pi / 4);
	camera cam(point3(4, 2, 3), point3(0, 0, -1), vUp, vFov, aspectRatio, time0, time1);

	for (int j = imageHeight-1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < imageWidth; ++i)
		{
			color pixelColors;
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				auto u = (double(i) + randomDouble()) / (imageWidth - 1);
				auto v = (double(j) + randomDouble())/ (imageHeight - 1);
				ray r = cam.get_ray(u, v);
				
				pixelColors += rayColor(r, world, maxDepth);
			}
			write_color(std::cout, pixelColors, samples_per_pixel );
		}
	}
	std::cerr << "\nDone.\n";
}