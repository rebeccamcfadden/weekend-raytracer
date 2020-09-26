#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

// color get_rgb(double r, double g, double b) {
//   // Write the translated [0,255] value of each color component.
//   return color(static_cast<int>(256 * clamp(r, 0.0, 0.999)),
//                static_cast<int>(256 * clamp(g, 0.0, 0.999)),
//                static_cast<int>(256 * clamp(b, 0.0, 0.999)));
// }

// color get_translated_color(color pixel_color, int spp) {
//   auto r = pixel_color.x();
//   auto g = pixel_color.y();
//   auto b = pixel_color.z();
//   auto scale = 1.0 / spp;
//   r *= scale;
//   g *= scale;
//   b *= scale;
//   return get_rgb(r, g, b);
// }

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples.
  auto scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif