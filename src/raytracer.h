#pragma once

#include <array>

#include "vec3.h"

#include "camera.h"
#include "color.h"
#include "image.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"

constexpr int width = 100;
constexpr int height = 100;
constexpr int samples = 10;

constexpr Vec3 trace (const Ray& r, World& world)
{
	HitRecord rec = world.hit (r, 0.0, std::numeric_limits<float>::max ());
	if (rec.hit)
	{
		// Lambertian
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere (world.random);
		return 0.5 * trace (Ray (rec.p, target - rec.p), world);

		// hit something, color with uv
		// return 0.5 * (rec.normal + 1.0f);
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
	world.add_shape (new Sphere (Vec3 (0, -1.5, -3), 2));
	world.add_shape (new Sphere (Vec3 (0, 1, -1), 0.5));

	Camera cam{
		Vec3 (0, 0, 0), Vec3 (0, 0, -1), Vec3 (0, 1, 0), 90, static_cast<float> (width) / static_cast<float> (height)
	};

	Image<Color, x_size, y_size> framebuffer{};
	for (int i = 0; i < x_size; i++)
	{
		for (int j = 0; j < y_size; j++)
		{
			Vec3 color (0, 0, 0);
			for (int s = 0; s < sample_count; s++)
			{
				float u = float (i + x_offset + world.random.get_float ()) / float (width);
				float v = float (j + y_offset + world.random.get_float ()) / float (height);
				Ray r = cam.get_ray (u, v);
				color += trace (r, world);
			}
			color /= static_cast<float> (sample_count);

			framebuffer.set (i, j, vec_to_color (color));
		}
	}
	return framebuffer;
}
