#pragma once

#include "vec3.h"

class ray
{
	public:
	constexpr ray () {}
	constexpr ray (const vec3& a, const vec3& b)
	{
		A = a;
		B = b;
	}
	constexpr vec3 origin () const { return A; }
	constexpr vec3 direction () const { return B; }
	constexpr vec3 point_at_parameter (float t) const { return A + B * t; }

	vec3 A;
	vec3 B;
};