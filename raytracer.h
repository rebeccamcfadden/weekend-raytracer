#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <stdlib.h>
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


// Headers

#include "ray.h"
#include "vec3.h"

#endif