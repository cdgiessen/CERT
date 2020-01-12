#pragma once

#include "ray.h"
#include "vec3.h"

struct Light
{
	constexpr Light (Vec3 color) : color (color) {}
	constexpr virtual ~Light ();
	Vec3 color;
};

constexpr Light::~Light () {}

struct PointLight : public Light
{
	constexpr PointLight (Vec3 color, Vec3 position) : Light (color), position (position) {}
	Vec3 position;
};

struct DirLight : public Light
{
	constexpr DirLight (Vec3 color, Vec3 direction) : Light (color), direction (direction) {}

	Vec3 direction;
};

struct SpotLight : public Light
{
	constexpr SpotLight (Vec3 color, Vec3 position, Vec3 direction, float cutoff)
	: Light (color), position (position), direction (direction), cutoff (cutoff)
	{
	}
	Vec3 position;
	Vec3 direction;
	float cutoff;
};