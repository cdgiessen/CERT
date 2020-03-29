#pragma once

#include "dynamic_array.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "shape.h"
#include "util.h"

namespace cert
{
constexpr Vec3 background_color (Vec3 direction)
{
	Vec3 unit_direction = normalize (direction);
	float t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * Vec3 (1.0, 1.0, 1.0) + t * Vec3 (0.5, 0.7, 1.0);
}

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
	constexpr Texture* add_texture (Texture* texture)
	{
		textures.push_back (texture);
		return texture;
	}
	constexpr HitRecord hit (const Ray& r, float t_min, float t_max) const
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

	constexpr bool intersects (const Ray& r, float t_min, float t_max) const
	{
		for (int i = 0; i < shapes.size (); i++)
		{
			bool hit = shapes.at (i)->intersects (r, t_min, t_max);
			if (hit) return true;
		}
		return false;
	}


	constexpr Vec3 light_hit (Vec3 point, Vec3 normal) const
	{
		Vec3 attenuation = VEC3_ZERO;
		for (int i = 0; i < lights.size (); i++)
		{
			auto rec = lights.at (i)->visible (point, normal);
			if (rec.visible)
			{
				float distance = (lights.at (i)->position - point).length ();
				if (!intersects (lights.at (i)->ray_to_light (point), 0.001, distance))
				{
					attenuation += rec.attenuation;
				}
			}
		}
		return attenuation;
	}

	PRNG random;

	private:
	DynamicArray<Shape*> shapes;
	DynamicArray<Material*> materials;
	DynamicArray<Light*> lights;
	DynamicArray<Texture*> textures;
};
} // namespace cert