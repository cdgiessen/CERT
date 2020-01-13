#pragma once

#include "dynamic_array.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "util.h"

struct HitRecord
{
	bool hit;
	float t;
	Vec3 p;
	Vec3 normal;
	Material* mat;
};


struct Shape
{
	constexpr Shape () {}
	constexpr Shape (Material* mat) : mat (mat) {}
	constexpr virtual ~Shape ();
	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const = 0;
	Material* mat = nullptr;
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
		for (int i = 0; i < materials.size (); i++)
		{
			delete materials.at (i);
		}
		for (int i = 0; i < lights.size (); i++)
		{
			delete lights.at (i);
		}
	}

	constexpr Shape* add_shape (Shape* shape)
	{
		shapes.push_back (shape);
		return shape;
	}
	constexpr Material* add_material (Material* mat)
	{
		materials.push_back (mat);
		return mat;
	}
	constexpr Light* add_light (Light* light)
	{
		lights.push_back (light);
		return light;
	}


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
	DynamicArray<Material*> materials;
	DynamicArray<Light*> lights;
};
