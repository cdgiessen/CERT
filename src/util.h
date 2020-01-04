#pragma once

#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

template <class T> constexpr T const& min (const T& a, const T& b) { return (b < a) ? b : a; }
template <class T> constexpr T const& max (const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T> constexpr T abs (T num) { return num >= 0 ? num : -num; }

struct PRNG
{
	constexpr PRNG (int seed = 123456789) : seed (seed) {}

	constexpr int get_int ()
	{
		seed = (1103515245 * seed + 12345) % std::numeric_limits<int>::max ();
		return static_cast<int> (seed);
	}
	constexpr float get_float ()
	{
		seed = ((1103515245 * seed + 12345) % std::numeric_limits<int>::max ());
		return static_cast<float> (seed) / static_cast<float> (std::numeric_limits<int>::max ());
	}

	private:
	long seed;
};

constexpr bool nearly_equal (const float a,
    const float b,
    float epsilon = 128 * std::numeric_limits<float>::epsilon (),
    float relth = std::numeric_limits<float>::min ())
// those defaults are arbitrary and could be removed
{
	// return (a < b + 0.001 && a > b - 0.001);

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

// Taken from http://brnz.org/hbr/?p=1518

// Based on code from
// https://graphics.stanford.edu/~seander/bithacks.html
constexpr int count_leading_zeroes (uint64_t v)
{                // clang-format off
  constexpr char bit_position[64] = {  
     0,  1,  2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 34, 20, 40, 
     5, 17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57, 
    63,  6, 12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56, 
    62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58 };
	             // clang-format on
	v |= v >> 1; // first round down to one less than a power of 2
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v = (v >> 1) + 1;

	return 63 - bit_position[(v * 0x0218a392cd3d5dbf) >> 58]; // [3]
}

constexpr int32_t bits (float f)
{
	if (f == 0.0f)
		return 0; // also matches -0.0f and gives wrong result
	else if (f == std::numeric_limits<float>::infinity ())
		return 0x7f800000;
	else if (f == -std::numeric_limits<float>::infinity ())
		return 0xff800000;
	else if (f != f)       // NaN
		return 0x7fc00000; // This is my NaN...

	bool sign = f < 0.0f;
	float abs_f = sign ? -f : f;

	int exponent = 254;

	while (abs_f < 0x1p87f)
	{
		abs_f *= 0x1p41f;
		exponent -= 41;
	}

	uint64_t a = (uint64_t) (abs_f * 0x1p-64f);
	int lz = count_leading_zeroes (a);
	exponent -= lz;

	if (exponent <= 0)
	{
		exponent = 0;
		lz = 8 - 1;
	}

	int32_t significand = (a << (lz + 1)) >> (64 - 23); // [3]
	return (sign << 31) | (exponent << 23) | significand;
}

constexpr float bits (int32_t i)
{
	int sign = (i >> 31);
	int exponent = ((i & 0x7f800000) >> 23) - 127;
	int mantissa = (i & 0x007fffff);
	float out = sign == 1 ? 1.0 : -1.0;
	int negative = 0;
	while (exponent > 0)
	{
		out *= 2;
		exponent = exponent >> 1;
	}
	return out;
}

template <class To, class From>
typename std::enable_if<(sizeof (To) == sizeof (From)) && std::is_trivially_copyable<From>::value && std::is_trivial<To>::value,
    // this implementation requires that To is trivially default constructible
    To>::type
// constexpr support needs compiler magic
bit_cast (const From& src) noexcept
{
	To dst;
	// need some UB free way to do memcpy at compile time
	std::memcpy (&dst, &src, sizeof (To));
	return dst;
}



inline float fast_sqrt (float number)
{
	int32_t i = 0;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = bit_cast<int, float> (y);
	// i = *static_cast<int*> (&y); // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1); // what the fuck?
	// y = *static_cast<int*> (&i);
	y = bit_cast<int, float> (i);
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

constexpr bool feq (float x, float y)
{
	return abs (x - y) <= std::numeric_limits<float>::epsilon ();
}

constexpr float trig_series (float x, float sum, float n, int i, int s, float t)
{
	return feq (sum, sum + t * s / n) ?
	           sum :
	           trig_series (x, sum + t * s / n, n * i * (i + 1), i + 2, -s, t * x * x);
}

constexpr float sin (float x) { return trig_series (x, x, 6.0f, 4, -1, x * x * x); }
constexpr float cos (float x) { return trig_series (x, 1.0f, 2.0f, 3, -1, x * x); }
constexpr float tan (float x) { return sin (x) / cos (x); }