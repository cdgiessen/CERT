#pragma once

#include <array>

#include "vec3.h"

#include "camera.h"
#include "color.h"
#include "image.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "world.h"

constexpr int thread_count = 12;

constexpr int width = 10 * thread_count;
constexpr int height = 10 * thread_count;
constexpr int samples = 3; // this value is squared!
constexpr int max_bounces = 5;

namespace cert
{
constexpr void setup_scene (World& world)
{
	auto grey = world.add_material (new Lambertian (Vec3 (0.5, 0.5, 0.5)));
	world.add_shape (new Sphere (Vec3 (0, -1000, 0), 1000, grey));

	int x_range = 3;
	int z_range = 3;
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
					auto lambert = world.add_material (
					    new Lambertian (Vec3 (world.random.get_float () * world.random.get_float (),
					        world.random.get_float () * world.random.get_float (),
					        world.random.get_float () * world.random.get_float ())));
					world.add_shape (new Sphere (center, 0.2, lambert));
				}
				else if (choose_mat < 0.95)
				{ // metal
					auto metal = world.add_material (new Metal (Vec3 (0.5 * (1 + world.random.get_float ()),
					    0.5 * (1 + world.random.get_float ()),
					    0.5 * (1 + world.random.get_float ()))));
					world.add_shape (new Sphere (center, 0.2, metal));
				}
				// else
				//{ // glass
				//	auto glass = world.add_material (new Dielectric (1.5));
				//	world.add_shape (new Sphere (center, 0.2, glass));
				//}
			}
		}
	}



	auto col1 = world.add_material (new Metal (Vec3 (0.4, 1.0, 0.6)));
	auto col2 = world.add_material (new Metal (Vec3 (0.4, 0.2, 0.1)));
	auto col3 = world.add_material (new Metal (Vec3 (0.7, 0.6, 0.5)));
	auto glass = world.add_material (new Dielectric (1.5f));

	world.add_shape (new Sphere (Vec3 (-2, 1, 0), 1.0, col1));
	world.add_shape (new Sphere (Vec3 (0, 1, 0), 1.0, glass));
	world.add_shape (new Sphere (Vec3 (2, 1, 0), 1.0, col3));

	world.add_light (new PointLight (Vec3 (2, 4, 2), VEC3_ONE, 15.0f));
	world.add_light (new PointLight (Vec3 (2, 4, -2), VEC3_ONE, 15.0f));
	world.add_light (new PointLight (Vec3 (-2, 4, 2), VEC3_ONE, 15.0f));
	world.add_light (new PointLight (Vec3 (-2, 4, -2), VEC3_ONE, 15.0f));
}

constexpr void setup_simple_scene (World& world)
{
	auto grey = world.add_material (new Lambertian (Vec3 (0.5, 0.5, 0.5)));
	auto col1 = world.add_material (new Lambertian (Vec3 (0.4, 1.0, 0.6)));

	world.add_shape (new Sphere (Vec3 (0, -1000, 0), 1000, grey));
	world.add_shape (new Sphere (Vec3 (0, 1, 0), 1.0, col1));

	world.add_light (new PointLight (Vec3 (1, 4, -2), VEC3_ONE, 10.0f));
	world.add_light (new PointLight (Vec3 (-1, 3, 2), VEC3_ONE, 10.0f));
}

constexpr Vec3 trace (const Ray& r, World& world, int depth)
{
	HitRecord rec = world.hit (r, 0.001, std::numeric_limits<float>::max ());
	if (rec.hit)
	{
		if (depth > max_bounces) return VEC3_ZERO;

		auto light_contribution = world.light_hit (rec.p, rec.normal);

		ScatterOut out = rec.mat->scatter (rec.p, rec.normal, rec.uv, r, world.random);
		if (out.index >= 0.0f)
		{
			if (out.index >= 1.0f)
			{
				return trace (out.refracted, world, depth + 1);
			}
			else
			{
				Vec3 reflect_contrib = trace (out.scattered, world, depth + 1);
				Vec3 refract_contrib = trace (out.refracted, world, depth + 1);
				return out.index * reflect_contrib + (1.0 - out.index) * refract_contrib;
			}
		}
		else if (out.is_scattered)
		{
			return light_contribution * out.attenuation * trace (out.scattered, world, depth + 1);
		}
		else
		{
			return light_contribution * out.attenuation;
		}
	}
	else
	{
		return background_color (r.direction);
	}
}

template <std::size_t width, std::size_t height, std::size_t x_size, std::size_t y_size, std::size_t x_offset, std::size_t y_offset, std::size_t sample_count>
constexpr auto raytrace ()
{
	World world{};
	// setup_simple_scene (world);
	setup_scene (world);
	Vec3 lookfrom{ 0, 1, 3 };
	Vec3 lookat{ 0, 0, 0 };
	Camera cam{ lookfrom, lookat, VEC3_UP, 90, static_cast<float> (width) / static_cast<float> (height) };

	OutputImage<Color, x_size, y_size> framebuffer{};
	for (int i = 0; i < x_size; i++)
	{
		for (int j = 0; j < y_size; j++)
		{
			Vec3 color (0, 0, 0);
			for (float sX = 0; sX < 1; sX += 1.0 / sample_count)
			{
				for (float sY = 0; sY < 1; sY += 1.0 / sample_count)
				{
					float u = float (i + x_offset + sX) / float (width);
					float v = float (j + y_offset + sY) / float (height);

					Ray r = cam.get_ray (u, v);
					color += trace (r, world, 0);
				}
			}
			color /= static_cast<float> (sample_count * sample_count);

			framebuffer.set (i, j, vec_to_color (color));
		}
	}
	return framebuffer;
}
} // namespace cert