#include "../src/util.h"

constexpr void run_util_test ()
{
	constexpr float p_1 = 0.1f;
	constexpr float p_2 = 0.2f;

	static_assert (nearly_equal (p_1 + p_2, 0.3f));
	static_assert (p_1 + p_2 == 0.3f);

	static_assert (nearly_equal (0.0f, 0.00000000000000000000000000000000000001f));
	static_assert (nearly_equal (0.1f, 0.100003f));
	static_assert (nearly_equal (0.1f, 0.099997f));
	static_assert (nearly_equal (0.5f, 0.50001f));
	static_assert (nearly_equal (0.5f, 0.49999f));
	static_assert (nearly_equal (1.0f, 1.00003f));
	static_assert (nearly_equal (1.0f, 0.99997f));
	static_assert (nearly_equal (2.0f, 2.00006f));
	static_assert (nearly_equal (2.0f, 1.99994f));
	static_assert (nearly_equal (4.0f, 4.0001f));
	static_assert (nearly_equal (8.0f, 8.0002f));
	static_assert (nearly_equal (10.0f, 10.0002f));
	static_assert (nearly_equal (1000.0f, 1000.01f));
	static_assert (nearly_equal (100000.0f, 100002.f));

	static_assert (nearly_equal (-0.0f, -0.00000000000000000000000000000000000001f));
	static_assert (nearly_equal (-0.1f, -0.100003f));
	static_assert (nearly_equal (-0.1f, -0.099997f));
	static_assert (nearly_equal (-0.5f, -0.50001f));
	static_assert (nearly_equal (-0.5f, -0.49999f));
	static_assert (nearly_equal (-1.0f, -1.00003f));
	static_assert (nearly_equal (-1.0f, -0.99997f));
	static_assert (nearly_equal (-2.0f, -2.00006f));
	static_assert (nearly_equal (-2.0f, -1.99994f));
	static_assert (nearly_equal (-4.0f, -4.0001f));
	static_assert (nearly_equal (-8.0f, -8.0002f));
	static_assert (nearly_equal (-10.0f, -10.0002f));
	static_assert (nearly_equal (-1000.0f, -1000.01f));
	static_assert (nearly_equal (-100000.0f, -100002.f));


	static_assert (nearly_equal (sqrt (1.0f), 1.0f));
	constexpr float two_s = sqrt (4.0f);
	static_assert (nearly_equal (two_s, 2.0f));
	constexpr float three_s = sqrt (9.0f);
	static_assert (nearly_equal (three_s, 3.0f));
	constexpr float four_s = sqrt (16.0f);
	static_assert (nearly_equal (four_s, 4.0f));
	constexpr float five_s = sqrt (25.0f);
	static_assert (nearly_equal (five_s, 5.0f));
}