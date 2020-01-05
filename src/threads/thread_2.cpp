#include "threads.h"

constexpr Image<Color, width, height / 12> frame =
    raytrace<width, height, width, height / 12, 0, 2 * height / 12, samples> ();

Image<Color, width, height / 12> get_frame_2 () { return frame; }
