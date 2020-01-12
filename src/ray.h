#pragma once

#include "util.h"
#include "vec3.h"

class Ray
{
	public:
	constexpr Ray () {}
	constexpr Ray (const Vec3& origin, const Vec3& direction)
	: origin (origin), direction (direction)
	{
	}
	constexpr Vec3 point_at_parameter (float t) const { return origin + direction * t; }

	Vec3 origin;
	Vec3 direction;
};

constexpr Vec3 random_in_unit_sphere (PRNG& random)
{
	int count = 0;
	Vec3 p;
	do
	{
		count++;
		p = 2.0 * Vec3 (random.get_float (), random.get_float (), random.get_float ()) - Vec3 (1, 1, 1);
	} while (p.squared_length () >= 1.0 && count < 100);
	return p;
}