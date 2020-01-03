#pragma once

#include "dynamic_array.h"
#include "ray.h"
#include "util.h"

struct HitRecord
{
	bool hit;
	float t;
	Vec3 p;
	Vec3 normal;
};

struct Shape
{
	constexpr virtual ~Shape ();
	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const = 0;
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

	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const
	{
		HitRecord return_rec{};
		return_rec.hit = false;
		float closest_so_far = t_max;
		for (int i = 0; i < shapes.size (); i++)
		{
			HitRecord temp_rec = shapes.at (i)->hit (r, t_min, closest_so_far);
			if (temp_rec.hit && temp_rec.t < closest_so_far)
			{
				closest_so_far = temp_rec.t;
				return_rec = temp_rec;
			}
		}
		return return_rec;
	}

	PRNG random;

	private:
	DynamicArray<Shape*> shapes;
};
