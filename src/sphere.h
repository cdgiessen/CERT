#pragma once

#include "shape.h"

class Sphere : public Shape
{
	public:
	constexpr Sphere () {}
	constexpr Sphere (Vec3 cen, float r, Material* mat) : Shape (mat), center (cen), radius (r)
	{
		this->mat = mat;
	};

	constexpr virtual bool intersects (const Ray& r, float t_min, float t_max) const
	{
		Vec3 oc = r.origin - center;
		float a = dot (r.direction, r.direction);
		float b = dot (oc, r.direction);
		float c = dot (oc, oc) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant <= 0) return false;
		float negative = (-b - sqrt (discriminant)) / a;
		if (negative < t_max && negative > t_min)
		{
			return true;
		}
		float positive = (-b + sqrt (discriminant)) / a;
		if (positive < t_max && positive > t_min)
		{
			return true;
		}
		return false;
	}

	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const
	{
		Vec3 oc = r.origin - center;
		float a = dot (r.direction, r.direction);
		float b = dot (oc, r.direction);
		float c = dot (oc, oc) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0)
		{
			float negative = (-b - sqrt (discriminant)) / a;
			if (negative < t_max && negative > t_min)
			{
				HitRecord out{};
				out.hit = true;
				out.t = negative;
				out.p = r.point_at_parameter (out.t);
				out.normal = normalize ((out.p - center));
				out.mat = mat;
				return out;
			}
			float positive = (-b + sqrt (discriminant)) / a;
			if (positive < t_max && positive > t_min)
			{
				HitRecord out{};
				out.hit = true;
				out.t = positive;
				out.p = r.point_at_parameter (out.t);
				out.normal = normalize ((out.p - center));
				out.mat = mat;
				return out;
			}
		}
		return HitRecord{ false, 0, { 0, 0, 0 }, { 0, 0, 0 } };
	}
	Vec3 center = VEC3_ZERO;
	float radius = 1.0;
};