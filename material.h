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

class refractor : public material {
 public:
  refractor(double ri) : ref_ind(ri) {}

  virtual bool scatter(const ray& r, const hit& rec, color& attenuation,
                       ray& scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double refract_ratio = rec.front ? (1.0 / ref_ind) : ref_ind;

    vec3 unit_dir = unit(r.direction());
    double cos_theta = fmin(-unit_dir.dot(rec.N), 1.0);
    double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

    vec3 direction;
    if (refract_ratio * sin_theta > 1.0 || reflectance(cos_theta, refract_ratio) > random_double()) {
      // Reflect
      direction = reflect(unit_dir, rec.N);
    } else {
      // Refract
      direction = refract(unit_dir, rec.N, refract_ratio);
    }
    scattered = ray(rec.p, direction);
    return true;
  }

 public:
  double ref_ind;

 private:
  static double reflectance(double cosine, double ref_ind) {
    // Schlick's approx
    auto r0 = (1 - ref_ind) / (1 + ref_ind);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine), 5);
  }
};

#endif