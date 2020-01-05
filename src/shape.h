#pragma once

#include "dynamic_array.h"
#include "material.h"
#include "ray.h"
#include "util.h"

struct Shape
{
	constexpr Shape () {}
	constexpr Shape (Material* mat) : mat (mat) {}
	constexpr virtual ~Shape ();
	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const = 0;
	Material* mat = nullptr;
};

constexpr Shape::~Shape () { delete mat; }

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
