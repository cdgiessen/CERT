#include "threads.h"

constexpr cert::OutputImage<cert::Color, width, height / 12> frame =
    cert::raytrace<width, height, width, height / 12, 0, 8 * height / 12, samples> ();

cert::OutputImage<cert::Color, width, height / 12> get_frame_8 () { return frame; }
