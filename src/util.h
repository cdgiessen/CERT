#pragma once

#include <limits>
#include <new>

template <class T> constexpr T const& min (const T& a, const T& b) { return (b < a) ? b : a; }
template <class T> constexpr T const& max (const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T> constexpr T abs (T num) { return num >= 0 ? num : -num; }

constexpr bool nearly_equal (const float a,
    const float b,
    float epsilon = 128 * std::numeric_limits<float>::epsilon (),
    float relth = std::numeric_limits<float>::min ())
// those defaults are arbitrary and could be removed
{
	// assert (std::numeric_limits<float>::epsilon () <= epsilon);
	// assert (epsilon < 1.f);

	if (a == b) return true;

	const auto diff = abs (a - b);
	const auto norm = min ((abs (a) + abs (b)), std::numeric_limits<float>::max ());
	return diff < max (relth, epsilon * norm);
}

constexpr float sqrt (float res)
{
	float l = 1;
	float r = res;

	while (!nearly_equal (l, r))
	{
		const auto mid = (r + l) / 2;
		if (mid * mid >= res)
		{
			r = mid;
		}
		else
		{
			l = mid + 1;
		}
	}
	return r;
}

template <typename T> class DynArr
{
	T* data = nullptr;
	std::size_t size = 0;

	public:
	constexpr DynArr () {}

	constexpr void resize (std::size_t new_size)
	{
		T* temp = new float[new_size];
		for (int i = 0; i < size; i++)
		{
			if (i < new_size) temp[i] = data[i];
		}
		delete[] data;
		size = new_size;
		data = temp;
	}


	T const& at (std::size_t index) const { return data[index]; }

	//~DynArr () { delete[] data; }
};