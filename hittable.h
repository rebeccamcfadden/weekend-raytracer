#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit {
    pt3 p;
    vec3 N;
    double t;

    bool front;
    inline void set_outward_normal(const ray& r, const vec3& normal) {
        front = r.direction().dot(normal) < 0;
        N = front ? normal : -normal;
    }
};

class hittable {
    public:
        virtual bool is_hit(const ray& r, double t_min, double t_max, hit& rec) const = 0;
};

#endif