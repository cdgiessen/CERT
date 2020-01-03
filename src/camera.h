#pragma once

#include "ray.h"

class Camera
{
	public:
	constexpr Camera ()
	{
		lower_left_corner = Vec3 (-2.0, -1.0, -1.0);
		horizontal = Vec3 (4.0, 0.0, 0.0);
		vertical = Vec3 (0.0, 2.0, 0.0);
		origin = Vec3 (0.0, 0.0, 0.0);
	}

	constexpr Ray get_ray (float u, float v)
	{
		return Ray (origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};