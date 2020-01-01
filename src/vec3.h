#pragma once

#include <stdlib.h>

#include "util.h"

struct vec3
{
	float x = 0;
	float y = 0;
	float z = 0;

	constexpr vec3 () : x (0), y (0), z (0) {}
	constexpr vec3 (float x, float y, float z) : x (x), y (y), z (z) {}

	const vec3& operator+ () const { return *this; }
	vec3 operator- () const { return vec3 (-x, -y, -z); }

	vec3& operator+= (const vec3& v2);
	vec3& operator-= (const vec3& v2);
	vec3& operator*= (const vec3& v2);
	vec3& operator/= (const vec3& v2);
	vec3& operator*= (const float t);
	vec3& operator/= (const float t);

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

constexpr vec3 operator+ (const vec3& v1, const vec3& v2)
{
	return vec3 (v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
constexpr vec3 operator- (const vec3& v1, const vec3& v2)
{
	return vec3 (v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
constexpr vec3 operator* (const vec3& v1, const vec3& v2)
{
	return vec3 (v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
constexpr vec3 operator/ (const vec3& v1, const vec3& v2)
{
	return vec3 (v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}
constexpr vec3 operator* (const vec3& v1, const float t)
{
	return vec3 (v1.x * t, v1.y * t, v1.z * t);
}
constexpr vec3 operator/ (const vec3& v1, const float t)
{
	return vec3 (v1.x / t, v1.y / t, v1.z / t);
}
constexpr vec3 operator* (const float t, const vec3& v1)
{
	return vec3 (v1.x * t, v1.y * t, v1.z * t);
}

inline vec3& vec3::operator+= (const vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline vec3& vec3::operator-= (const vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline vec3& vec3::operator*= (const vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline vec3& vec3::operator*= (const float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

inline vec3& vec3::operator/= (const vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline vec3& vec3::operator/= (const float t)
{
	float k = 1.0 / t;

	x *= k;
	y *= k;
	z *= k;
	return *this;
}

constexpr inline float dot (const vec3& v1, const vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

constexpr inline vec3 cross (const vec3& v1, const vec3& v2)
{
	return vec3 (v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

constexpr inline vec3 unit_vector (const vec3 v) { return v / v.length (); }
