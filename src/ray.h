#pragma once

#include "util.h"
#include "vec3.h"

class Ray
{
	public:
	constexpr Ray (const Vec3& origin, const Vec3& direction)
	: origin (origin), direction (direction)
	{
	}
	constexpr Vec3 point_at_parameter (float t) const { return origin + direction * t; }

	Vec3 origin = VEC3_ZERO;
	Vec3 direction = VEC3_ZERO;
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

class AABB
{
	public:
	AABB (Vec3 const& mininum, Vec3 const& maximum) : mininum (mininum), maximum (maximum) {}

	bool hit (Ray const& r, float tmin, float tmax) const
	{
		for (int a = 0; a < 3; a++)
		{
			float t0 = min ((mininum[a] - r.origin[a]) / r.direction[a],
			    (maximum[a] - r.origin[a]) / r.direction[a]);
			float t1 = max ((mininum[a] - r.origin[a]) / r.direction[a],
			    (maximum[a] - r.origin[a]) / r.direction[a]);
			tmin = max (t0, tmin);
			tmax = min (t1, tmax);
			if (tmax <= tmin) return false;
		}
		return true;
	}

	Vec3 mininum;
	Vec3 maximum;
};