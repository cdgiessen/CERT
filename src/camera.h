#pragma once

#include "ray.h"
namespace cert
{

class Camera
{
	public:
	constexpr Camera (Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect)
	{
		Vec3 u, v, w;
		float theta = vfov * 3.141 / 180;
		float half_height = tan (theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = normalize (lookfrom - lookat);
		u = normalize (cross (vup, w));
		v = cross (w, u);
		upper_left_corner = origin - half_width * u + half_height * v - w;
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
	}

	constexpr Ray get_ray (float u, float v)
	{
		return Ray (origin, upper_left_corner + u * horizontal - v * vertical - origin);
	}

	Vec3 origin;
	Vec3 upper_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};
}