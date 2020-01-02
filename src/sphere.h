#pragma once

#include "shape.h"

class Sphere : public Shape
{
	public:
	constexpr Sphere () {}
	constexpr Sphere (Vec3 cen, float r) : center (cen), radius (r){};
	constexpr virtual bool hit (const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		Vec3 oc = r.origin () - center;
		float a = dot (r.direction (), r.direction ());
		float b = dot (oc, r.direction ());
		float c = dot (oc, oc) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0)
		{
			float temp = (-b - sqrt (discriminant)) / a;
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter (rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
			temp = (-b + sqrt (discriminant)) / a;
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter (rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
		}
		return false;
	}
	Vec3 center = ZERO;
	float radius = 1.0;
};