#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"

class camera {
 public:
  camera(pt3 lookfrom, pt3 lookat, vec3 vup, double fov, double aspect_ratio,
         double aperature, double focus_dist) {
    auto theta = deg2rad(fov);
    auto h = tan(theta / 2);
    double view_height = 2.0 * h;
    double view_width = aspect_ratio * view_height;

    w = unit(lookfrom - lookat);
    u = unit(vup.cross(w));
    v = w.cross(u);

    origin = lookfrom;
    horizontal = focus_dist * view_width * u;
    vertical = focus_dist * view_height * v;
    lower_left = origin - (horizontal / 2) - (vertical / 2) - focus_dist * w;

    lens_radius = aperature / 2;
  }

  ray get_ray(double s, double t) const {
    vec3 rd = lens_radius * random_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();

    return ray(origin + offset,
               lower_left + s * horizontal + t * vertical - origin - offset);
  }

 private:
  pt3 origin;
  pt3 lower_left;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double lens_radius;
};

#endif