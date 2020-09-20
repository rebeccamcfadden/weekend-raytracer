#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

using namespace std;

class hittable_list : public hittable {
 public:
  hittable_list() {}
  hittable_list(shared_ptr<hittable> obj) { add(obj); }

  void clear() { objects.clear(); }
  void add(shared_ptr<hittable> obj) { objects.push_back(obj); }

  virtual bool is_hit(const ray& r, double tmin, double tmax,
                   hit& rec) const override;

 public:
  vector<shared_ptr<hittable>> objects;
};

bool hittable_list::is_hit(const ray& r, double tmin, double tmax, hit& rec) const {
    hit temp;
    bool hit_anything = false;
    double closest = tmax;

    for (const auto& obj: objects) {
        if(obj->is_hit(r, tmin, closest, temp)) {
            hit_anything = true;
            closest = temp.t;
            rec = temp;
        }
    }

    return hit_anything;
}

#endif