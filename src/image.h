#pragma once

#include "vec3.h"

template <typename T, std::size_t width, std::size_t height> struct Image
{
	constexpr Image () {}
	constexpr void set (int x, int y, T const& value) { data[y * width + x] = value; }
	constexpr T get (int x, int y) const { return data[y * width + x]; }
	constexpr T at (int index) const { return data[index]; }
	constexpr std::size_t size () const { return data.size (); }

	private:
	std::array<T, width * height> data;
};