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

 private:
  double data[3];
};

// Type aliases
using pt3 = vec3;    // point in 3D space
using color = vec3;  // RGB

#endif