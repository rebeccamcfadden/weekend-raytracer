#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

class camera {
 public:
  camera() {
    // Image dimensions
    const double aspect_ratio = 16.0 / 9.0;
    // Camera location
    double view_height = 2.0;
    double view_width = aspect_ratio * view_height;
    double focal_length = 1.0;

    origin = pt3(0, 0, 0);
    horizontal = vec3(view_width, 0, 0);
    vertical = vec3(0, view_height, 0);
    lower_left = origin - (horizontal / 2) - (vertical / 2) - vec3(0, 0, focal_length);
  }

  ray get_ray(double u, double v) const {
      return ray(origin, lower_left + u*horizontal + v*vertical - origin);
  }

 private:
  pt3 origin;
  pt3 lower_left;
  vec3 horizontal;
  vec3 vertical;
};

#endif