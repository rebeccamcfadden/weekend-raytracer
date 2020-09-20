#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"

class sphere : public hittable {
 public:
  sphere() {}
  sphere(pt3 cen, double rad) : center(center), radius(rad){};
  virtual bool is_hit(const ray& r, double tmin, double tmax,
                      hit& rec) const override;

 public:
  pt3 center;
  double radius;
};

bool sphere::is_hit(const ray& r, double tmin, double tmax, hit& rec) const {
  // GENERAL IDEA:
  // (p - center) dot (p - center) = rad^2
  // p = r.origin + t*r.direction
  // quadratic formula : x = (-b +/- sqrt(b^2 - 4ac))/2a
  // discriminant : b^2 - 4ac
  // a = r.direction dot r.direction
  // b = 2 * (r.direction dot (r.origin - center))
  // c = (r.origin - center) dot (r.origin - center) - rad ^2

  vec3 oc = r.origin() - center;  // origin - center
  double a = r.direction().length_sq();
  double b = oc.dot(r.direction());
  double c = oc.length_sq() - radius * radius;
  double discriminant = b * b - a * c;

  if (discriminant > 0) {
    double root = sqrt(discriminant);

    double t = (-b - root) / a;
    if (t < tmax && t > tmin) {
      rec.t = t;
      rec.p = r.at(rec.t);
      vec3 normal = (rec.p - center) / radius;
      rec.set_outward_normal(r, normal); 
      return true;
    }

    double t = (-b + root) / a;
    if (t < tmax && t > tmin) {
      rec.t = t;
      rec.p = r.at(rec.t);
      vec3 normal = (rec.p - center) / radius;
      rec.set_outward_normal(r, normal); 
      return true;
    }
  }
  return false;
}

#endif