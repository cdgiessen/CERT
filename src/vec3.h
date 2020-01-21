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

	constexpr float operator[] (const int index) const
	{
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		return std::numeric_limits<float>::signaling_NaN ();
	}

	const Vec3& operator+ () const { return *this; }
	constexpr Vec3 operator- () const { return Vec3 (-x, -y, -z); }

	constexpr Vec3& operator+= (const Vec3& v2);
	constexpr Vec3& operator-= (const Vec3& v2);
	constexpr Vec3& operator*= (const Vec3& v2);
	constexpr Vec3& operator/= (const Vec3& v2);
	constexpr Vec3& operator*= (const float t);
	constexpr Vec3& operator/= (const float t);

	constexpr inline float length () const { return sqrt (x * x + y * y + z * z); }
	constexpr inline float squared_length () const { return x * x + y * y + z * z; }
	constexpr inline Vec3& norm ()
	{
		float k = 1.0 / sqrt (x * x + y * y + z * z);
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
};

constexpr Vec3 operator+ (const Vec3& v1, const Vec3& v2)
{
	return Vec3 (v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
constexpr Vec3 operator+ (const Vec3& v1, const int& t)
{
	return Vec3 (v1.x + t, v1.y + t, v1.z + t);
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
constexpr Vec3 operator* (const float t, const Vec3& v1) { return operator* (v1, t); }

constexpr Vec3& Vec3::operator+= (const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

constexpr Vec3& Vec3::operator-= (const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

constexpr Vec3& Vec3::operator*= (const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

constexpr Vec3& Vec3::operator*= (const float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

constexpr Vec3& Vec3::operator/= (const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

constexpr Vec3& Vec3::operator/= (const float t)
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

constexpr Vec3 reflect (const Vec3& v, const Vec3& n) { return v - 2 * dot (v, n) * n; }

constexpr inline Vec3 normalize (const Vec3& v) { return v / v.length (); }

constexpr static Vec3 VEC3_ZERO = Vec3 (0.0f, 0.0f, 0.0f);
constexpr static Vec3 VEC3_ONE = Vec3 (1.0f, 1.0f, 1.0f);
constexpr static Vec3 VEC3_RIGHT = Vec3 (1.0f, 0.0f, 0.0f);
constexpr static Vec3 VEC3_LEFT = Vec3 (-1.0f, 0.0f, 0.0f);
constexpr static Vec3 VEC3_UP = Vec3 (0.0f, 1.0f, 0.0f);
constexpr static Vec3 VEC3_DOWN = Vec3 (0.0f, -1.0f, 0.0f);
constexpr static Vec3 VEC3_FORWARD = Vec3 (0.0f, 0.0f, 1.0f);
constexpr static Vec3 VEC3_BACKWARD = Vec3 (0.0f, 0.0f, -1.0f);
