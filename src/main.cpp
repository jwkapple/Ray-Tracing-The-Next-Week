
#include "rtweekend.h"

#include "bvh.h"
#include "material.h"
#include "hittableList.h"

#include "sphere.h"
#include "movingSphere.h"
#include "aarect.h"
#include "box.h"

#include "translate.h"
#include "rotateY.h"

#include "color.h"
#include "camera.h"
#include "texture.h"

#include <iostream>



vec3 rayColor(const ray &r,const color& background, const hittable& world, int depth)
{
	hitRecord rec;
	
	// If we've exceeded the ray bounce limit, no mor light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// if ray does'nt hit anything, return black background
	if (!world.hit(r, 0.001, infinity, rec))
		return background;
	
	color attenuation;
	ray scattered;
	color emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

	// If object doesn't scatters ray(which means it emit), return emitted color
	if (!rec.matPtr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * rayColor(scattered, background, world, depth - 1);
}

hittableList randomScene()
{
	hittableList world;

	auto checker = make_shared<checkerTexture>
		(
			make_shared<solidColor>(0.2, 0.3, 0.1),
			make_shared<solidColor>(0.9)
		);

	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -3; a < 3; a++) {
		for (int b = -3; b < 3; b++) {
			auto choose_mat = randomDouble();
			point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					auto texture = make_shared<solidColor>(albedo);
					sphere_material = make_shared<lambertian>(texture);
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
	
	auto tmpTexture = make_shared<solidColor>(0.5, 0.5, 0.);
		world.add(make_shared<sphere>(
			point3(0, -1000, 0), 1000, make_shared<lambertian>(tmpTexture)));
	
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	tmpTexture = make_shared<solidColor>(.4, .2, .1);
	world.add(
		make_shared<sphere>(point3(-4, 1, 0), 1.0, make_shared<lambertian>(tmpTexture)));

	world.add(
		make_shared<sphere>(point3(4, 1, 0), 1.0, make_shared<metal>(color(.7, .6, .5))));

		return world;
}

hittableList twoPerlinSpheres()
{
	hittableList objects;
	double scale = 3.0;
	auto pertext = make_shared<noiseTexture>(scale);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), -2, make_shared<lambertian>(pertext)));

	return objects;
}

hittableList earth()
{
	hittableList objects;

	auto earthTexture = make_shared<imageTexture>("samples/earthmap.jpg");
	auto earthSurface = make_shared<lambertian>(earthTexture);
	auto earthGlobe = make_shared<sphere>(point3(0,2,0), 2, earthSurface);
	objects.add(earthGlobe);

	auto diffLight = make_shared<diffuseLight>(make_shared<solidColor>(4, 4, 4));
	objects.add(make_shared<sphere>(point3(0, 7, 0), 2, diffLight));
	objects.add(make_shared<xyRect>(3, 5, 1, 3, -2, diffLight));
	
	double scale = 3.0;
	auto pertext = make_shared<noiseTexture>(scale);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	
	return objects;
}

hittableList cornellBox()
{
	hittableList objects;

	auto red = make_shared<lambertian>(make_shared<solidColor>(.65, .05, .05));
	auto white = make_shared<lambertian>(make_shared<solidColor>(.73, .73, .73));
	auto green = make_shared<lambertian>(make_shared<solidColor>(.12, .45, .15));
	auto light = make_shared<diffuseLight>(make_shared<solidColor>(15, 15, 15));

	objects.add(make_shared<yzRect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yzRect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xzRect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<xzRect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xzRect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xyRect>(0, 555, 0, 555, 555, white));

	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotateY>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(point3(0), point3(165), white);
	box2 = make_shared<rotateY>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2);

	return objects;
}
int main()
{
	const auto aspectRatio = 16.0 / 9.0;
	const int imageWidth = 384;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samples_per_pixel = 100;
	const int maxDepth = 50;
	point3 lookFrom = point3(278, 278, -800);
	point3 lookAt = point3(278, 278, 0);
	const vec3 vUp = vec3(0, 1, 0);
	const double vFov = 40;
	const double time0 = 0.0;
	const double time1 = 1.0;
	const color background = color(0);

	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	hittableList world = cornellBox();

	auto R = cos(pi / 4);
	camera cam(lookFrom, lookAt, vUp, vFov, aspectRatio, time0, time1);

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
				
				pixelColors += rayColor(r, background, world, maxDepth);
			}
			write_color(std::cout, pixelColors, samples_per_pixel );
		}
	}
	std::cerr << "\nDone.\n";
}