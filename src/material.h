#pragma once

#include "ray.h"
#include "vec3.h"

struct ScatterOut
{
	bool is_scattered;
	Vec3 attenuation;
	Ray scattered = Ray (VEC3_ZERO, VEC3_ONE);
	float index = -1.0f;
	Ray refracted = Ray (VEC3_ZERO, VEC3_ONE);
};

struct Material
{
	constexpr Material (Vec3 albedo) : albedo (albedo) {}
	constexpr virtual ~Material ();

	constexpr virtual ScatterOut scatter (Vec3 point, Vec3 normal, const Ray& r_in, PRNG& random) const = 0;

	Vec3 albedo;
};

constexpr Material::~Material () {}


struct Lambertian : public Material
{
	constexpr Lambertian (const Vec3& a) : Material (a) {}
	constexpr virtual ScatterOut scatter (Vec3 point, Vec3 normal, const Ray& r_in, PRNG& random) const
	{
		return { .is_scattered = false, .attenuation = albedo };
	}
};

struct Metal : public Material
{
	constexpr Metal (const Vec3& a) : Material (a) {}
	constexpr virtual ScatterOut scatter (Vec3 point, Vec3 normal, const Ray& r_in, PRNG& random) const
	{
		Vec3 reflected = reflect (normalize (r_in.direction), normal);
		return { .is_scattered = dot (reflected, normal) > 0.0,
			.attenuation = albedo,
			.scattered = Ray (point, reflected) };
	}
};

constexpr float schlick (float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * my_pow ((1 - cosine), 5);
}

struct RefractDataOut
{
	bool is_refracted = false;
	Vec3 refracted;
};

constexpr RefractDataOut refract_func (Vec3 const& v, Vec3 const& n, float ni_over_nt)
{
	Vec3 uv = normalize (v);
	float dt = dot (uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		return { true, ni_over_nt * (uv - n * dt) - n * sqrt (discriminant) };
	}
	else
		return { false };
}

struct Dielectric : public Material
{

	constexpr Dielectric (float ri) : Material (VEC3_ZERO), ref_idx (ri) {}
	constexpr virtual ScatterOut scatter (Vec3 point, Vec3 normal, const Ray& r_in, PRNG& random) const
	{
		Vec3 outward_normal;
		Vec3 reflected = reflect (r_in.direction, normal);
		float ni_over_nt = 0;
		float reflect_prob = 0;
		float cosine = 0;

		if (dot (r_in.direction, normal) > 0)
		{
			outward_normal = -normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot (r_in.direction, normal) / r_in.direction.length ();
		}
		else
		{
			outward_normal = normal;
			ni_over_nt = 1.0 / ref_idx;
		}
		RefractDataOut refracted = refract_func (r_in.direction, outward_normal, ni_over_nt);
		if (refracted.is_refracted)
		{
			reflect_prob = schlick (cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0;
		}

		return {
			true, Vec3 (1.0, 1.0, 1.0), Ray (point, reflected), reflect_prob, Ray (point, refracted.refracted)
		};
	}

	float ref_idx;
};

struct UV : public Material
{
	constexpr UV () : Material (VEC3_ZERO) {}
	constexpr virtual ScatterOut scatter (Vec3 point, Vec3 normal, const Ray& r_in, PRNG& random) const
	{
		return { .is_scattered = false, .attenuation = 0.5 * (normal + 1.0f) };
	}
};