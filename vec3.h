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
  inline double dot(const vec3 &v) {
    return (data[0] * v[0]) + (data[1] * v[1]) + (data[2] * v[2]);
  }

  // cross product
  inline vec3 cross(const vec3 &v) {
    return vec3((data[1] * v[2]) - (data[2] * v[1]),
                -((data[0] * v[2]) - (data[2] * v[0])),
                (data[0] * v[1]) - (data[1] * v[0]));
  }
  friend ostream& operator<<(ostream& os, vec3 &v);

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
inline vec3 unit(const vec3 &u) {
    return u / u.length();
}

#endif