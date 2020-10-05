#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

class camera {
 public:
  camera(pt3 lookfrom, pt3 lookat, vec3 vup, double fov, double aspect_ratio) {
    auto theta = deg2rad(fov);
    auto h = tan(theta / 2);
    double view_height = 2.0 * h;
    double view_width = aspect_ratio * view_height;

    auto w = unit(lookfrom - lookat);
    auto u = unit(vup.cross(w));
    auto v = w.cross(u);

    origin = lookfrom;
    horizontal = view_width * u;
    vertical = view_height * v;
    lower_left = origin - (horizontal / 2) - (vertical / 2) - w;
  }

  ray get_ray(double s, double t) const {
    return ray(origin, lower_left + s * horizontal + t * vertical - origin);
  }

 private:
  pt3 origin;
  pt3 lower_left;
  vec3 horizontal;
  vec3 vertical;
};

#endif