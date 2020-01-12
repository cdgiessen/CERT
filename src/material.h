#pragma once

#include "ray.h"
#include "vec3.h"

struct Material;

struct HitRecord
{
	bool hit;
	float t;
	Vec3 p;
	Vec3 normal;
	Material* mat;
};

struct Material
{
	constexpr Material (Vec3 albedo) : albedo (albedo) {}
	constexpr virtual ~Material ();
	constexpr virtual bool scatter (
	    const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered, PRNG& random) const = 0;

	Vec3 albedo;
};

constexpr Material::~Material () {}


struct Lambertian : public Material
{
	constexpr Lambertian (const Vec3& a) : Material (a) {}
	constexpr virtual bool scatter (
	    const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered, PRNG& random) const
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere (random);
		scattered = Ray (rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

struct Metal : public Material
{
	constexpr Metal (const Vec3& a, float f) : Material (a), fuzz (f)
	{
		if (f >= 1) fuzz = 1;
	}
	constexpr virtual bool scatter (
	    const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered, PRNG& random) const
	{
		Vec3 reflected = reflect (unit_vector (r_in.direction), rec.normal);
		scattered = Ray (rec.p, reflected);
		attenuation = albedo;
		return (dot (scattered.direction, rec.normal) > 0);
	}
	float fuzz;
};

constexpr float schlick (float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow ((1 - cosine), 5);
}

constexpr bool refract (const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)
{
	Vec3 uv = unit_vector (v);
	float dt = dot (uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt (discriminant);
		return true;
	}
	else
		return false;
}

struct Dielectric : public Material
{

	constexpr Dielectric (float ri) : Material (VEC3_ZERO), ref_idx (ri) {}
	constexpr virtual bool scatter (
	    const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered, PRNG& random) const
	{
		Vec3 outward_normal;
		Vec3 reflected = reflect (r_in.direction, rec.normal);
		float ni_over_nt = 0;
		attenuation = Vec3 (1.0, 1.0, 1.0);
		Vec3 refracted;

		float reflect_prob = 0;
		float cosine = 0;

		if (dot (r_in.direction, rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot (r_in.direction, rec.normal) / r_in.direction.length ();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot (r_in.direction, rec.normal) / r_in.direction.length ();
		}

		if (refract (r_in.direction, outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick (cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0;
		}

		if (random.get_float () < reflect_prob)
		{
			scattered = Ray (rec.p, reflected);
		}
		else
		{
			scattered = Ray (rec.p, refracted);
		}

		return true;
	}

	float ref_idx;
};

struct UV : public Material
{
	constexpr UV () : Material (VEC3_ZERO) {}
	constexpr virtual bool scatter (
	    const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered, PRNG& random) const
	{
		attenuation = 0.5 * (rec.normal + 1.0f);
		return false;
	}
};