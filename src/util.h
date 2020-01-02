#pragma once

#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

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

template <typename T> class DynArr
{
	T* m_data = nullptr;
	std::size_t m_size = 0;
	std::size_t m_allocated = 0;

	public:
	constexpr DynArr ()
	{
		m_allocated = 10;
		m_data = new T[m_allocated];
		m_size = 0;
	}
	constexpr DynArr (std::size_t size)
	{
		m_data = new T[size];
		m_size = size;
		m_allocated = size;
	}
	constexpr ~DynArr ()
	{
		if (m_data != nullptr) delete[] m_data;
	}
	DynArr (DynArr const& obj) = delete;
	DynArr& operator= (DynArr const& obj) = delete;

	DynArr (DynArr&& obj) : m_data (obj.m_data), m_size (obj.m_size), m_allocated (obj.m_allocated)
	{
		obj.m_data = nullptr;
	}
	DynArr& operator= (DynArr&& obj)
	{
		m_data = obj.m_data;
		m_size = obj.m_size;
		m_allocated = obj.m_allocated;
		obj.m_data = nullptr;
		return *this;
	}

	constexpr void resize (std::size_t new_size)
	{
		if (new_size > m_allocated)
		{
			T* temp = new float[m_allocated * 2];
			for (int i = 0; i < m_size; i++)
			{
				temp[i] = m_data[i];
			}
			delete[] m_data;
			m_data = temp;
			m_size = new_size;
			m_allocated = m_allocated * 2;
		}
		else
		{
			m_size = new_size;
		}
	}

	constexpr T& set (std::size_t index, T value)
	{
		if (index < m_size)
		{
			m_data[index] = value;
			return m_data[index];
		}
	}

	constexpr void push_back (T value)
	{
		m_data[m_size] = value;
		m_size++;
	}

	constexpr void erase (std::size_t index) {}

	constexpr void reset () { m_size = 0; }

	constexpr T const& at (std::size_t index) const { return m_data[index]; }
	constexpr std::size_t size () const { return m_size; }
	constexpr T* data () const { return m_data; }
};