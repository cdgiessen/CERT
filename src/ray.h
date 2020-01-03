#pragma once

#include "util.h"
#include "vec3.h"

class Ray
{
	public:
	constexpr Ray () {}
	constexpr Ray (const Vec3& a, const Vec3& b)
	{
		A = a;
		B = b;
	}
	constexpr Vec3 origin () const { return A; }
	constexpr Vec3 direction () const { return B; }
	constexpr Vec3 point_at_parameter (float t) const { return A + B * t; }

	Vec3 A;
	Vec3 B;
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