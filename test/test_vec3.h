#include "../src/vec3.h"

constexpr void run_vec3_test ()
{
	constexpr auto zero = cert::VEC3_ZERO;
	constexpr auto v_one = cert::VEC3_ONE;

	static_assert (zero.x == 0.0f && zero.y == 0.0f && zero.z == 0.0f);
	static_assert (v_one[0] == 1.0f && v_one[1] == 1.0f && v_one[2] == 1.0f);
	constexpr auto neg_one = -v_one;
	static_assert (neg_one.x == -1.0f && neg_one.y == -1.0f && neg_one.z == -1.0f);

	constexpr auto len_pre_v1 = cert::Vec3 (0, 3, 4);
	constexpr auto len_pre_v2 = cert::Vec3 (3, 0, 4);
	constexpr auto len_pre_v3 = cert::Vec3 (3, 4, 0);

	static_assert (len_pre_v1.squared_length () == 25.0f); // && len_pre_v1.length () == 5.0f);
	static_assert (len_pre_v2.squared_length () == 25.0f); // && len_pre_v2.length () == 5.0f);
	static_assert (len_pre_v3.squared_length () == 25.0f); // && len_pre_v3.length () == 5.0f);
}