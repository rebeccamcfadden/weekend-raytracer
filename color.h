#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

color get_rgb(color pixel_color) {
    // Write the translated [0,255] value of each color component.
    return color(
        static_cast<int>(pixel_color[0] * 255.99),
        static_cast<int>(pixel_color[1] * 255.99),
        static_cast<int>(pixel_color[2] * 255.99));
}

#endif