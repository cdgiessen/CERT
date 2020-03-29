#pragma once

#include "ray.h"
#include "vec3.h"

namespace cert
{
struct LightRayRecord
{
	bool visible;
	Vec3 attenuation;
};

struct Light
{
	constexpr Light (Vec3 position, Vec3 color) : position (position), color (color) {}
	constexpr virtual ~Light ();
	constexpr Ray ray_to_light (Vec3 const& point) { return { point, position - point }; }
	constexpr virtual LightRayRecord visible (Vec3 const& point, Vec3 const& normal) = 0;
	Vec3 position;
	Vec3 color;
};

constexpr Light::~Light () {}

struct PointLight : public Light
{
	constexpr PointLight (Vec3 position, Vec3 color, float strength)
	: Light (position, color), strength (strength)
	{
	}
	constexpr LightRayRecord visible (Vec3 const& point, Vec3 const& normal) override
	{
		Vec3 direction = position - point;
		float distance = direction.length ();
		float lambert = dot (normal, normalize (direction));

		return { .visible = lambert > 0.0f, .attenuation = color * lambert * strength / (distance * distance) };
	}
	float strength;
};

struct SpotLight : public Light
{
	constexpr SpotLight (Vec3 position, Vec3 color, Vec3 direction, float strength, float cutoff)
	: Light (position, color), direction (direction), strength (strength), cutoff (cutoff)
	{
	}
	constexpr virtual bool visible (Ray const& ray)
	{
		return dot (ray.direction, direction) > 0.0f && (ray.origin - position).length () < strength;
	}

	Vec3 direction;
	float strength;
	float cutoff;
};
} // namespace cert