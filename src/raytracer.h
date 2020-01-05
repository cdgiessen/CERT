#pragma once

#include <array>

#include "vec3.h"

#include "camera.h"
#include "color.h"
#include "image.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"

constexpr int width = 150;
constexpr int height = 150;
constexpr int samples = 10;
constexpr int max_bounces = 10;

constexpr void setup_scene (World& world)
{
	int n = 50;
	world.add_shape (new Sphere (Vec3 (0, -1000, 0), 1000, new Lambertian (Vec3 (0.5, 0.5, 0.5))));
	int i = 1;
	int x_range = 1;
	int z_range = 1;
	for (int a = -x_range; a < x_range; a++)
	{
		for (int b = -z_range; b < z_range; b++)
		{
			float choose_mat = world.random.get_float ();
			Vec3 center (a + 0.9 * world.random.get_float (), 0.2, b + 0.9 * world.random.get_float ());
			if ((center - Vec3 (4, 0.2, 0)).length () > 0.9)
			{
				if (choose_mat < 0.8)
				{ // diffuse
					world.add_shape (new Sphere (center,
					    0.2,
					    new Lambertian (Vec3 (world.random.get_float () * world.random.get_float (),
					        world.random.get_float () * world.random.get_float (),
					        world.random.get_float () * world.random.get_float ()))));
				}
				else if (choose_mat < 0.95)
				{ // metal
					world.add_shape (new Sphere (center,
					    0.2,
					    new Metal (Vec3 (0.5 * (1 + world.random.get_float ()),
					                   0.5 * (1 + world.random.get_float ()),
					                   0.5 * (1 + world.random.get_float ())),
					        0.5 * world.random.get_float ())));
				}
				else
				{ // glass
					world.add_shape (new Sphere (center, 0.2, new Dielectric (1.5)));
				}
			}
		}
	}

	world.add_shape (new Sphere (Vec3 (0, 1, 0), 1.0, new Dielectric (1.5)));
	world.add_shape (new Sphere (Vec3 (-4, 1, 0), 1.0, new Lambertian (Vec3 (0.4, 0.2, 0.1))));
	world.add_shape (new Sphere (Vec3 (4, 1, 0), 1.0, new Metal (Vec3 (0.7, 0.6, 0.5), 0.0)));
}

constexpr Vec3 trace (const Ray& r, World& world, int depth)
{
	HitRecord rec = world.hit (r, 0.001, std::numeric_limits<float>::max ());
	if (rec.hit)
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < max_bounces && rec.mat->scatter (r, rec, attenuation, scattered, world.random))
		{
			return attenuation * trace (scattered, world, depth + 1);
		}
		else
		{
			return Vec3 (0, 0, 0);
		}
	}
	else
	{
		// hit nothing
		Vec3 unit_direction = unit_vector (r.direction ());
		float t = 0.5 * (unit_direction.y + 1.0);
		return (1.0 - t) * Vec3 (1.0, 1.0, 1.0) + t * Vec3 (0.5, 0.7, 1.0);
	}
}

template <std::size_t width, std::size_t height, std::size_t x_size, std::size_t y_size, std::size_t x_offset, std::size_t y_offset, std::size_t sample_count>
constexpr auto raytrace ()
{
	World world{};
	setup_scene (world);
	// world.add_shape (new Sphere (Vec3 (0, -1.5, -3), 2, new Lambertian (Vec3 (0.5, 0.5, 0.5))));
	// world.add_shape (new Sphere (Vec3 (0, 1, -1), 0.5, new Lambertian (Vec3 (0.5, 0.5, 0.5))));
	// world.add_shape (new Sphere (Vec3 (0.5, 1, -1), 0.5, new UV ()));
	Vec3 lookfrom{ 13, 2, 3 };
	Vec3 lookat = VEC3_ZERO;
	Camera cam{ lookfrom, lookat, VEC3_UP, 90, static_cast<float> (width) / static_cast<float> (height) };

	Image<Color, x_size, y_size> framebuffer{};
	for (int i = 0; i < x_size; i++)
	{
		for (int j = 0; j < y_size; j++)
		{
			Vec3 color (0, 0, 0);
			for (int s = 0; s < sample_count; s++)
			{
				float u = float (i + x_offset /*+ world.random.get_float ()*/) / float (width);
				float v = float (j + y_offset /*+ world.random.get_float ()*/) / float (height);
				Ray r = cam.get_ray (u, v);
				color += trace (r, world, 0);
			}
			color /= static_cast<float> (sample_count);

			framebuffer.set (i, j, vec_to_color (color));
		}
	}
	return framebuffer;
}
