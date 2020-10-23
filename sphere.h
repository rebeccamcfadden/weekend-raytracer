#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"

class sphere : public hittable {
 public:
  sphere() {}
  sphere(pt3 cen, double rad, shared_ptr<material> m)
      : center(cen), radius(rad), matPtr(m){};
  virtual bool is_hit(const ray& r, double tmin, double tmax,
                      hit& rec) const override;
  virtual bool bounding_box(double t0, double t1,
                            aabb& bound_box) const override;
  
 public:
  pt3 center;
  double radius;
  shared_ptr<material> matPtr;
};

void get_sphere_uv(const vec3& p, double& u, double& v) {
  auto theta = atan2(p.z(), p.x());
  auto phi = asin(p.y());
  u = 1-(theta + pi) / (2*pi);
  v = (phi + pi/2) / pi;
}

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
  auto a = r.direction().length_sq();
  auto b = oc.dot(r.direction());
  auto c = oc.length_sq() - radius * radius;
  auto discriminant = (b * b) - (a * c);

  if (discriminant > 0) {
    auto root = sqrt(discriminant);

    auto t = (-b - root) / a;
    if (t < tmax && t > tmin) {
      rec.t = t;
      rec.p = r.at(rec.t);
      vec3 normal = (rec.p - center) / radius;
      rec.set_outward_normal(r, normal);
      get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
      rec.matPtr = matPtr;
      // cerr << "HIT: object with center " << center << endl;
      return true;
    }

    t = (-b + root) / a;
    if (t < tmax && t > tmin) {
      rec.t = t;
      rec.p = r.at(rec.t);
      vec3 normal = (rec.p - center) / radius;
      rec.set_outward_normal(r, normal);
      get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
      rec.matPtr = matPtr;
      // cerr << "HIT: object with center " << center << endl;
      return true;
    }
  }
  return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& bound_box) const {
  bound_box = aabb(center - vec3(radius, radius, radius),
                   center + vec3(radius, radius, radius));
  return true;
}

#endif