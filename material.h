#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "raytracer.h"

class material {
 public:
  virtual bool scatter(const ray& r, const hit& rec, color& attenuation,
                       ray& scattered) const = 0;
};

class diffuse : public material {
 public:
  diffuse(const color& a) : albedo(a) {}

  virtual bool scatter(const ray& r, const hit& rec, color& attenuation,
                       ray& scattered) const override {
    vec3 scatter_dir = rec.N + random_unit_vector();
    scattered = ray(rec.p, scatter_dir);
    attenuation = albedo;
    return true;
  }

 public:
  color albedo;
};

class metal : public material {
 public:
  metal(const color& a, double f) : albedo(a), fuzz(f) {}
  virtual bool scatter(const ray& r, const hit& rec, color& attenuation,
                       ray& scattered) const override {
    vec3 reflected = reflect(unit(r.direction()), rec.N);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (scattered.direction().dot(rec.N) > 0);
  }

 public:
  color albedo;
  double fuzz;
};

#endif