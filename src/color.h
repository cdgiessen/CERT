#pragma once
#include <stdint.h>

#include "vec3.h"
struct Color
{
	float r = 0, g = 0, b = 0;
};

constexpr Color vec_to_color (Vec3 const& v) { return { v.x, v.y, v.z }; }
