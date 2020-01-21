#pragma once
#include <stdint.h>

#include "vec3.h"
struct Color
{
	uint8_t r = 0, g = 0, b = 0;
};

constexpr Color vec_to_color (Vec3 const& v)
{
	return { static_cast<uint8_t> (clamp (v.x * 255.99, 0.0, 255.0)),
		static_cast<uint8_t> (clamp (v.y * 255.99, 0.0, 255.0)),
		static_cast<uint8_t> (clamp (v.z * 255.99, 0.0, 255.0)) };
}
