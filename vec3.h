#ifndef VEC3_H
#define VEC3_H

#include <math.h>

#include <iostream>

using namespace std;

class vec3 {
 public:
  vec3() : data{0, 0, 0} {}
  vec3(double x, double y, double z) : data{x, y, z} {}

  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double z() const { return data[2]; }

  // negate
  vec3 operator-() const { return vec3(-data[0], -data[1], -data[2]); }
  // get value at index
  double operator[](int i) const { return data[i]; }
  // get value at index
  double &operator[](int i) { return data[i]; }
  // addition (or subtraction using negate)
  vec3 &operator+=(const vec3 &v) {
    data[0] += v[0];
    data[1] += v[1];
    data[2] += v[2];
    return *this;
  }
  // multiplication by scalar
  vec3 &operator*=(const double t) {
    data[0] *= t;
    data[1] *= t;
    data[2] *= t;
    return *this;
  }
  // division by scalar
  vec3 &operator/=(const double t) { return *this *= (1 / t); }
  // 3D length of vector
  double length() const { return sqrt(length_sq()); }
  // 3D length of vector squared
  double length_sq() const {
    return (data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2]);
  }
  // dot product
  inline double dot(const vec3 &v) const {
    return (data[0] * v[0]) + (data[1] * v[1]) + (data[2] * v[2]);
  }

  // cross product
  inline vec3 cross(const vec3 &v) {
    return vec3((data[1] * v[2]) - (data[2] * v[1]),
                -((data[0] * v[2]) - (data[2] * v[0])),
                (data[0] * v[1]) - (data[1] * v[0]));
  }

  // random direction helpers
  inline static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  inline static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
  }

  friend ostream &operator<<(ostream &os, vec3 &v);

 private:
  double data[3];
};

// Type aliases
using pt3 = vec3;    // point in 3D space
using color = vec3;  // RGB

// Utility functions

// print
inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  out << v[0] << ' ' << v[1] << ' ' << v[2];
  return out;
}
// add vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}
// subtract vectors
inline vec3 operator-(const vec3 &u, const vec3 &v) { return u + -v; }

// multiply vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}
// scalar multiplication
inline vec3 operator*(const double t, const vec3 &u) {
  return vec3(u[0] * t, u[1] * t, u[2] * t);
}
inline vec3 operator*(const vec3 &u, const double t) { return t * u; }
// division
inline vec3 operator/(const vec3 &u, const double t) { return (1 / t) * u; }

// unit vector
inline vec3 unit(const vec3 &u) { return u / u.length(); }

// get random vector in unit sphere
vec3 random_in_unit_sphere() {
  while (true) {
    vec3 p = vec3::random(-1, 1);
    if (p.length_sq() >= 1) continue;
    return p;
  }
}

// get random vector in unit sphere
vec3 random_unit_vector() {
  auto a = random_double(0, 2 * pi);
  auto z = random_double(-1, 1);
  auto r = sqrt(1 - z * z);
  return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_unit_disk() {
  while (true) {
    auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_sq() >= 1) continue;
    return p;
  }
}

vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * v.dot(n) * n; }

vec3 refract(const vec3& v, const vec3& n, double refract_ratio) {
  auto cos_theta = (-v).dot(n);
  vec3 r_perp = refract_ratio * (v + cos_theta * n);
  vec3 r_parallel = -sqrt(fabs(1.0 - r_perp.length_sq())) * n;
  return r_parallel + r_perp;
}

#endif