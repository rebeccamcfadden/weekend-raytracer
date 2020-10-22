#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <algorithm>

#include "hittable.h"
#include "hittable_list.h"
#include "raytracer.h"

class bvh_node : public hittable {
 public:
  bvh_node() {}
  bvh_node(const hittable_list& list, double time0, double time1)
      : bvh_node(list.objects, 0, list.objects.size(), time0, time1){};

  bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start,
           size_t end, double time0, double time1);
  virtual bool is_hit(const ray& r, double tmin, double tmax,
                      hit& rec) const override;
  virtual bool bounding_box(double t0, double t1,
                            aabb& bound_box) const override;

 public:
  shared_ptr<hittable> left;
  shared_ptr<hittable> right;
  aabb box;
};

bool bvh_node::is_hit(const ray& r, double tmin, double tmax, hit& rec) const {
  if (!box.hit(r, tmin, tmax)) {
    return false;
  }

  bool lefthit = left->is_hit(r, tmin, tmax, rec);
  bool righthit = right->is_hit(r, tmin, lefthit ? rec.t : tmax, rec);

  return lefthit || righthit;
}

bool bvh_node::bounding_box(double t0, double t1, aabb& bound_box) const {
  bound_box = box;
  return true;
}

bvh_node::bvh_node(const std::vector<shared_ptr<hittable>>& src_objects,
                   size_t start, size_t end, double time0, double time1) {
  auto objects = src_objects;  // modifiable

  int axis = random_int(0, 2);
  auto compare_function =
      (axis == 0) ? box_x_func : (axis == 1) ? box_y_func : box_z_func;
  size_t obj_span = end - start;

  if (obj_span == 1) {
    left = right = objects[start];
  } else if (obj_span == 2) {
    bool first = compare_function(objects[start], objects[start + 1]);
    left = first ? objects[start] : objects[start + 1];
    right = first ? objects[start + 1] : objects[start];
  } else {
    sort(objects.begin(), objects.end(), compare_function);

    int mid = floor((end - start) / 2) + start;
    left = make_shared<bvh_node>(objects, start, mid, time0, time1);
    right = make_shared<bvh_node>(objects, mid + 1, end, time0, time1);
  }

  aabb left_box, right_box;

  if (!left->bounding_box(time0, time1, left_box) ||
      !right->bounding_box(time0, time1, right_box)) {
    cerr << "No bounding box in bvh constructor" << endl;
  }

  box = surrounding_box(left_box, right_box);
}

inline bool box_compare(const shared_ptr<hittable> a,
                        const shared_ptr<hittable> b, int axis) {
  aabb boxA;
  aabb boxB;

  if (!a->bounding_box(0, 0, boxA) || !b->bounding_box(0, 0, boxB)) {
    cerr << "No bounding box in bvh constructor" << endl;
  }

  return boxA.min()[axis] < boxB.min()[axis];
}

bool box_x_func(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
  return box_compare(a, b, 0);
}

bool box_y_func(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
  return box_compare(a, b, 1);
}

bool box_z_func(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
  return box_compare(a, b, 2);
}

#endif