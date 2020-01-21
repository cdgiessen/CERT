#pragma once

#include "material.h"
#include "ray.h"
#include "vec3.h"

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
	constexpr virtual bool intersects (const Ray& r, float t_min, float t_max) const = 0;
	constexpr virtual HitRecord hit (const Ray& r, float t_min, float t_max) const = 0;
	Material* mat = nullptr;
};

constexpr Shape::~Shape () {}