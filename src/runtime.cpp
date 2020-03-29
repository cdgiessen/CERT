#include <fstream>
#include <iostream>

#include "raytracer.h"

template <typename T> void print_image (std::ofstream& ppm, T const& frame)
{
	for (int i = 0; i < frame.size (); i++)
	{
		int r = static_cast<int> (frame.at (i).r);
		int g = static_cast<int> (frame.at (i).g);
		int b = static_cast<int> (frame.at (i).b);

		ppm << r << " " << g << " " << b << "\n";
	}
}

void runtime ()
{
	std::ofstream ppm ("output_runtime.ppm");
	ppm << "P3\n" << width << " " << height << "\n255\n";
	print_image (ppm, cert::raytrace<width, height, width, height, 0, 0, samples> ());
}

int main (int argc, char** argv)
{
	std::ofstream ppm ("output_runtime.ppm");
	ppm << "P3\n" << width << " " << height << "\n255\n";
	print_image (ppm, cert::raytrace<width, height, width, height, 0, 0, samples> ());
}