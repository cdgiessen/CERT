#include "threads.h"

constexpr cert::OutputImage<cert::Color, width, height / 12> frame =
    cert::raytrace<width, height, width, height / 12, 0, 11 * height / 12, samples> ();

cert::OutputImage<cert::Color, width, height / 12> get_frame_11 () { return frame; }
