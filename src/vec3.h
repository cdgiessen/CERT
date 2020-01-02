#pragma once

#include <stdlib.h>

#include "util.h"

struct Vec3
{
	float x = 0;
	float y = 0;
	float z = 0;

	constexpr Vec3 () : x (0), y (0), z (0) {}
	constexpr Vec3 (float x, float y, float z) : x (x), y (y), z (z) {}

	const Vec3& operator+ () const { return *this; }
	Vec3 operator- () const { return Vec3 (-x, -y, -z); }

	Vec3& operator+= (const Vec3& v2);
	Vec3& operator-= (const Vec3& v2);
	Vec3& operator*= (const Vec3& v2);
	Vec3& operator/= (const Vec3& v2);
	Vec3& operator*= (const float t);
	Vec3& operator/= (const float t);

	constexpr inline float length () const { return sqrt (x * x + y * y + z * z); }
	constexpr inline float squared_length () const { return x * x + y * y + z * z; }
	constexpr inline void make_unit_vector ()
	{
		float k = 1.0 / length ();
		x *= k;
		y *= k;
		z *= k;
	}
};

constexpr Vec3 operator+ (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
constexpr Vec3 operator- (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
constexpr Vec3 operator* (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
constexpr Vec3 operator/ (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}
constexpr Vec3 operator* (const Vec3& v1, const float t)
{
	return Vec3 (v1.x * t, v1.y * t, v1.z * t);
}
constexpr Vec3 operator/ (const Vec3& v1, const float t)
{
	return Vec3 (v1.x / t, v1.y / t, v1.z / t);
}
constexpr Vec3 operator* (const float t, const Vec3& v1)
{
	return Vec3 (v1.x * t, v1.y * t, v1.z * t);
}

inline Vec3& Vec3::operator+= (const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vec3& Vec3::operator-= (const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vec3& Vec3::operator*= (const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline Vec3& Vec3::operator*= (const float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

inline Vec3& Vec3::operator/= (const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vec3& Vec3::operator/= (const float t)
{
	float k = 1.0 / t;

	x *= k;
	y *= k;
	z *= k;
	return *this;
}

constexpr inline float dot (const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

constexpr inline Vec3 cross (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

constexpr inline Vec3 unit_vector (const Vec3 v) { return v / v.length (); }

constexpr static Vec3 ZERO = Vec3 (0.0, 0.0, 0.0);