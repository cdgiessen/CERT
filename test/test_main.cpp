#include "test_ray.h"
#include "test_util.h"
#include "test_vec3.h"
#include <iostream>
constexpr int run_tests ()
{
	run_util_test ();
	run_vec3_test ();
	run_ray_test ();
	return 0;
}
int main ()
{
	constexpr int res = run_tests ();
	std::cout << my_pow (-2, -3) << "\n";
	return 0;
}