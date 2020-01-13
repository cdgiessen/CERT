#pragma once

#include "ray.h"
#include "vec3.h"

struct Light
{
	constexpr Light (Vec3 color) : color (color) {}
	constexpr virtual ~Light ();
	constexpr virtual bool visible (Ray const& ray) = 0;
	Vec3 color;
};

constexpr Light::~Light () {}

struct PointLight : public Light
{
	constexpr PointLight (Vec3 color, Vec3 position, float strength)
	: Light (color), position (position), strength (strength)
	{
	}
	constexpr virtual bool visible (Ray const& ray)
	{
		return (ray.origin - position).length () < strength;
	}
	Vec3 position;
	float strength;
};

struct DirLight : public Light
{
	constexpr DirLight (Vec3 color, Vec3 direction) : Light (color), direction (direction) {}
	constexpr virtual bool visible (Ray const& ray)
	{
		return dot (ray.direction, direction) > 0.0f;
	}

	Vec3 direction;
};

struct SpotLight : public Light
{
	constexpr SpotLight (Vec3 color, Vec3 position, Vec3 direction, float strength, float cutoff)
	: Light (color), position (position), direction (direction), strength (strength), cutoff (cutoff)
	{
	}
	constexpr virtual bool visible (Ray const& ray)
	{
		return dot (ray.direction, direction) > 0.0f && (ray.origin - position).length () < strength;
	}

	Vec3 position;
	Vec3 direction;
	float strength;
	float cutoff;
};