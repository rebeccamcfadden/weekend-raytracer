#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
 public:
  ray() {}
  ray(const pt3 &origin, const vec3 &direction) : orig(orig), dir(direction) {}

  pt3 origin() const { return orig; }
  vec3 direction() const { return dir; }

  pt3 at(double t) const { return orig + (t * dir); }

 public:
  pt3 orig;
  vec3 dir;
};

#endif