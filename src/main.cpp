#include <array>
#include <fstream>
#include <iostream>

#include "ray.h"
#include "vec3.h"

struct Color
{
	float r = 0, g = 0, b = 0;
};

constexpr Color vec_to_color (vec3 const& v) { return { v.x, v.y, v.z }; }

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

template <std::size_t t_width, std::size_t t_height>
constexpr std::array<Color, t_width * t_height> calc ()
{
	vec3 lower_left_corner (-2.0, -1.0, -1.0);
	vec3 horizontal (4.0, 0.0, 0.0);
	vec3 vertical (0.0, 2.0, 0.0);
	vec3 origin (0.0, 0.0, 0.0);


	std::array<Color, t_width * t_height> framebuffer{};
	for (int i = 0; i < t_width; i++)
	{
		for (int j = t_height - 1; j >= 0; j--)
		{
			float u = float (i) / float (t_width);
			float v = float (j) / float (t_height);
			ray r (origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color (r);


			framebuffer.at (j * t_width + i) = vec_to_color (col);
		}
	}
	return framebuffer;
}


int main ()
{
	constexpr int nx = 400;
	constexpr int ny = 200;
	constexpr auto frame = calc<nx, ny> ();

	std::ofstream ppm ("output.ppm");
	ppm << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			int ir = int (255.99 * frame.at (i * ny + j).r);
			int ig = int (255.99 * frame.at (i * ny + j).g);
			int ib = int (255.99 * frame.at (i * ny + j).b);

			ppm << ir << " " << ig << " " << ib << "\n";
		}
	}
}