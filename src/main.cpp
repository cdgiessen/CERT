#include <array>
#include <fstream>
#include <iostream>

#include "vec3.h"

#include "ray.h"

#include "color.h"

constexpr float hit_sphere (const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin () - center;
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

constexpr vec3 color (const ray& r)
{
	float t = hit_sphere (vec3 (0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		vec3 N = unit_vector (r.point_at_parameter (t) - vec3 (0, 0, -1));
		return 0.5 * vec3 (N.x + 1, N.y + 1, N.z + 1);
	}
	vec3 unit_direction = unit_vector (r.direction ());
	t = 0.5 * (unit_direction.y + 1.0);
	return (1.0f - t) * vec3 (1.0, 1.0, 1.0) + t * vec3 (0.5, 0.7, 1.0);
}

template <std::size_t width, std::size_t height>
using Framebuffer = std::array<Color, width * height>;

template <std::size_t width, std::size_t height> constexpr auto trace ()
{
	vec3 lower_left_corner = vec3 (-2.0, -1.0, -1.0);
	vec3 horizontal (4.0, 0.0, 0.0);
	vec3 vertical (0.0, 2.0, 0.0);
	vec3 origin (0.0, 0.0, 0.0);

	Framebuffer<width, height> framebuffer{};
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float u = float (i) / float (width);
			float v = float (j) / float (height);
			ray r (origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color (r);

			framebuffer.at (j * width + i) = vec_to_color (col);
		}
	}
	return framebuffer;
}

constexpr int nx = 500;
constexpr int ny = 500;
constexpr auto frame = trace<nx, ny> ();


int main ()
{
	std::ofstream ppm ("output.ppm");
	ppm << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < frame.size (); i++)
	{
		ppm << static_cast<int> (frame.at (i).r) << " " << static_cast<int> (frame.at (i).g) << " "
		    << static_cast<int> (frame.at (i).b) << "\n";
	}
}