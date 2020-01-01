#pragma once
#include <stdint.h>

struct Color
{
	uint8_t r = 0, g = 0, b = 0;
};

constexpr Color vec_to_color (vec3 const& v)
{
	return { static_cast<uint8_t> (v.x * 255.99),
		static_cast<uint8_t> (v.y * 255.99),
		static_cast<uint8_t> (v.z * 255.99) };
}
