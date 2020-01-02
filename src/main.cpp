#include <array>
#include <fstream>
#include <iostream>

#include "vec3.h"

#include "color.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"


constexpr float hit_sphere (const Vec3& center, float radius, const Ray& r)
{
	Vec3 oc = r.origin () - center;
	float a = dot (r.direction (), r.direction ());
	float b = 2.0 * dot (oc, r.direction ());
	float c = dot (oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt (discriminant)) / (2.0 * a);
	}
}

constexpr Vec3 trace (const Ray& r, World const& world)
{
	HitRecord rec;
	if (world.hit (r, 0.0, std::numeric_limits<float>::max (), rec))
	{
		// hit something, color with uv
		return 0.5 * Vec3 (rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
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

template <std::size_t width, std::size_t height> constexpr auto raytrace ()
{
	Vec3 lower_left_corner = Vec3 (-2.0, -1.0, -1.0);
	Vec3 horizontal (4.0, 0.0, 0.0);
	Vec3 vertical (0.0, 2.0, 0.0);
	Vec3 origin (0.0, 0.0, 0.0);

	World world{};
	world.add_shape (new Sphere (Vec3 (0, 0, -1), 0.5));
	world.add_shape (new Sphere (Vec3 (0, -1.5, -1), 1));

	Framebuffer<width, height> framebuffer{};
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float u = float (i) / float (width);
			float v = float (j) / float (height);
			Ray r (origin, lower_left_corner + u * horizontal + v * vertical);
			Vec3 color = trace (r, world);

			framebuffer.at (j * width + i) = vec_to_color (color);
		}
	}
	return framebuffer;
}



constexpr int nx = 100;
constexpr int ny = 50;
constexpr auto frame = raytrace<nx, ny> ();

int main ()
{
	std::ofstream ppm ("output.ppm");
	ppm << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < frame.size (); i++)
	{
		int r = static_cast<int> (frame.at (i).r * 255.99);
		int g = static_cast<int> (frame.at (i).g * 255.99);
		int b = static_cast<int> (frame.at (i).b * 255.99);

		ppm << r << " " << g << " " << b << "\n";
	}
}