#include <array>
#include <fstream>
#include <iostream>

#include "vec3.h"

#include "camera.h"
#include "color.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"

constexpr Vec3 trace (const Ray& r, World& world)
{
	HitRecord rec = world.hit (r, 0.0, std::numeric_limits<float>::max ());
	if (rec.hit)
	{
		// Lambertian
		// Vec3 target = rec.p + rec.normal + random_in_unit_sphere (world.random);
		// return 0.5 * trace (Ray (rec.p, target - rec.p), world);

		// hit something, color with uv
		return 0.5 * (rec.normal + 1.0f);
	}
	else
	{
		// hit nothing
		Vec3 unit_direction = unit_vector (r.direction ());
		float t = 0.5 * (unit_direction.y + 1.0);
		return (1.0 - t) * Vec3 (1.0, 1.0, 1.0) + t * Vec3 (0.5, 0.7, 1.0);
	}
}

template <std::size_t width, std::size_t height>
using Framebuffer = std::array<Color, width * height>;

template <std::size_t width, std::size_t height, std::size_t sample_count>
constexpr auto raytrace (bool run_time)
{
	World world{};
	world.add_shape (new Sphere (Vec3 (0, -1.5, -1), 1));
	world.add_shape (new Sphere (Vec3 (0, 0, -1), 0.5));

	Camera cam{};

	Framebuffer<width, height> framebuffer{};
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Vec3 color (0, 0, 0);
			for (int s = 0; s < sample_count; s++)
			{
				float u = float (i + world.random.get_float ()) / float (width);
				float v = float (j + world.random.get_float ()) / float (height);
				Ray r = cam.get_ray (u, v);
				color += trace (r, world);
			}
			color /= float (sample_count);

			framebuffer.at (j * width + i) = vec_to_color (color);
		}
	}
	return framebuffer;
}



constexpr int width = 100;
constexpr int height = 50;
constexpr int samples = 1;
constexpr auto frame = raytrace<width, height, samples> (false);

int main (int argc, char** argv)
{
	if (argc == 2)
	{
		bool runtime = strcmp (argv[1], "runtime");
		auto other_frame = raytrace<width, height, samples> (false);
	}

	std::ofstream ppm ("output.ppm");
	ppm << "P3\n" << width << " " << height << "\n255\n";
	for (int i = 0; i < frame.size (); i++)
	{
		int r = static_cast<int> (frame.at (i).r);
		int g = static_cast<int> (frame.at (i).g);
		int b = static_cast<int> (frame.at (i).b);

		ppm << r << " " << g << " " << b << "\n";
	}
}