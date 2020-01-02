#pragma once

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