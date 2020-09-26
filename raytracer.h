#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
#include <memory>

using namespace std;

// Constants

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility

inline double deg2rad(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return random_double() * (max-min) + min;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Headers

#include "ray.h"
#include "vec3.h"

#endif