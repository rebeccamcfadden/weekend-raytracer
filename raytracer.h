#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <stdlib.h>
#include <random>
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
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
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