#ifndef AABB_H
#define AABB_H

#include "raytracer.h"

class aabb {
 public:
  aabb() {}
  aabb(const pt3& a, const pt3& b) {
    _min = a;
    _max = b;
  }

  pt3 min() const { return _min; }
  pt3 max() const { return _max; }

  bool hit(const ray& r, double tmin, double tmax) const {
    for (int i = 0; i < 3; i++) {
      double t0 = fmin((_min[i] - r.origin()[i]) / r.direction()[i],
                       (_max[i] - r.origin()[i]) / r.direction()[i]);
      double t1 = fmax((_min[i] - r.origin()[i]) / r.direction()[i],
                       (_max[i] - r.origin()[i]) / r.direction()[i]);
      tmin = fmax(t0, tmin);
      tmax = fmin(t1, tmax);
      if (tmax <= tmin) {
        return false;
      }
    }
    return true;
  }

 public:
  pt3 _min;
  pt3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
  pt3 small(fmin(box0.min().x(), box1.min().x()),
            fmin(box0.min().y(), box1.min().y()),
            fmin(box0.min().z(), box1.min().z()));
  pt3 large(fmax(box0.max().x(), box1.max().x()),
            fmax(box1.max().y(), box0.max().y()),
            fmax(box0.max().z(), box1.max().z()));

  return aabb(small, large);
}

#endif