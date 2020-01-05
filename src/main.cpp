#include <fstream>
#include <iostream>

#include "raytracer.h"

#include "threads/thread_0.h"
#include "threads/thread_1.h"
#include "threads/thread_2.h"
#include "threads/thread_3.h"


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
	print_image (ppm, get_frame_0 ());
	print_image (ppm, get_frame_1 ());
	print_image (ppm, get_frame_2 ());
	print_image (ppm, get_frame_3 ());
}