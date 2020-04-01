#pragma once

#include "vec3.h"
#include "util.h"
#include "dynamic_array.h"

namespace cert
{
template <typename T, std::size_t width, std::size_t height> struct OutputImage
{
	constexpr OutputImage () {}
	constexpr void set (int x, int y, T const& value) { data[y * width + x] = value; }
	constexpr T get (int x, int y) const { return data[y * width + x]; }
	constexpr T at (int index) const { return data[index]; }
	constexpr std::size_t size () const { return data.size (); }

	private:
	std::array<T, width * height> data;
};

class Texture
{
	public:
	constexpr Texture (int width, int height) : width (width), height (height), tex (width * height)
	{
	}
	constexpr void set (int x, int y, Vec3 const& value) { tex.at (x * width + y) = value; }
	constexpr Vec3 get (int x, int y) const { return tex.at (x * width + y); }

	constexpr Vec3 sample (UV uv) { return tex.at (get_index (uv.u, uv.v)); }

	private:
	constexpr size_t get_index (float x, float y) const
	{
		return static_cast<size_t> (x * width) * width + static_cast<size_t> (y * height);
	}

	int width;
	int height;
	DynamicArray<Vec3> tex;
};

} // namespace cert