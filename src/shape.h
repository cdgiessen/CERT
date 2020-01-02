#pragma once

#include "ray.h"
#include "util.h"

struct HitRecord
{
	float t;
	Vec3 p;
	Vec3 normal;
};

struct Shape
{
	constexpr virtual bool hit (const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	constexpr virtual ~Shape ();
};

constexpr Shape::~Shape () {}

struct World
{
	constexpr World () {}
	constexpr ~World ()
	{
		for (int i = 0; i < shapes.size (); i++)
		{
			delete shapes.at (i);
		}
	}

	constexpr void add_shape (Shape* shape) { shapes.push_back (shape); }

	constexpr virtual bool hit (const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		HitRecord temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;
		for (int i = 0; i < shapes.size (); i++)
		{
			if (shapes.at (i)->hit (r, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	private:
	DynArr<Shape*> shapes;
};
