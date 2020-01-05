#include "thread_1.h"

constexpr Image<Color, width, height / 4> frame =
    raytrace<width, height, width, height / 4, 0, height / 2, samples> ();

Image<Color, width, height / 4> get_frame_2 () { return frame; }
