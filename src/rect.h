#include "shape.h"

namespace cert
{
class Rect : public Shape
{
	public:
	constexpr Rect () {}
	constexpr Rect (float _x0, float _x1, float _y0, float _y1, float _k, Material* mat)
	: x0 (_x0), x1 (_x1), y0 (_y0), y1 (_y1), k (_k), mp (mat){};
	constexpr override HitRecord hit (const Ray& r, float t0, float t1) const;
	constexpr virtual bool bounding_box (float t0, float t1, aabb& box) const
	{
		box = aabb (Vec3 (x0, y0, k - 0.0001), Vec3 (x1, y1, k + 0.0001));
		return true;
	}
	Material* mp;
	float x0, x1, y0, y1, k;
};

HitRecord Rect::hit (const Ray& r, float t0, float t1) const
{
	float t = (k - r.origin ().z) / r.direction ().z;
	if (t < t0 || t > t1) return false;
	float x = r.origin ().x + t * r.direction ().x;
	float y = r.origin ().y + t * r.direction ().y;
	if (x < x0 || x > x1 || y < y0 || y > y1) return false;
	HitRecord rec;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter (t);
	rec.normal = VEC3_FORWARD;
	return {};
}
} // namespace cert