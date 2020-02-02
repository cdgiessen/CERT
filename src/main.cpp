#include <fstream>
#include <iostream>

#include "raytracer.h"

#include "threads/threads.h"

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

int main (int argc, char** argv)
{
	std::ofstream ppm ("output.ppm");
	ppm << "P3\n" << width << " " << height << "\n255\n";
	print_image (ppm, get_frame_11 ());
	print_image (ppm, get_frame_10 ());
	print_image (ppm, get_frame_9 ());
	print_image (ppm, get_frame_8 ());
	print_image (ppm, get_frame_7 ());
	print_image (ppm, get_frame_6 ());
	print_image (ppm, get_frame_5 ());
	print_image (ppm, get_frame_4 ());
	print_image (ppm, get_frame_3 ());
	print_image (ppm, get_frame_2 ());
	print_image (ppm, get_frame_1 ());
	print_image (ppm, get_frame_0 ());
}