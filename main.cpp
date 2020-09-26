#include <iostream>

#include "raytracer.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray &r, const hittable& world) {
  hit rec;
  if (world.is_hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.N + color(1, 1, 1));
  }
  vec3 unit_dir = unit(r.direction());
  double t = 0.5 * (unit_dir.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // Image dimensions
  const double aspect_ratio = 16.0 / 9.0;
  const int imgwidth = 800;
  const int imgheight = static_cast<int>(imgwidth / aspect_ratio);

  // World
  hittable_list world;
  world.add(make_shared<sphere>(pt3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(pt3(0, -100.5, -1), 100));
  world.add(make_shared<sphere>(pt3(2, 0, -1), 2));

  // Camera location
  double view_height = 2.0;
  double view_width = aspect_ratio * view_height;
  double focal_length = 1.0;

  pt3 origin = pt3(0, 0, 0);
  vec3 horizontal = vec3(view_width, 0, 0);
  vec3 vertical = vec3(0, view_height, 0);
  vec3 lower_left =
      origin - (horizontal / 2) - (vertical / 2) - vec3(0, 0, focal_length);

  cout << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";
  // u vector is x dir
  // v vector is y dir
  // to draw from top left, have to go off u = 0, v = imgheight
  for (int j = imgheight - 1; j >= 0; j--) {
    cerr << "\r" << j << " lines remaining." << flush;
    for (int i = 0; i < imgwidth; i++) {
      double u = double(i) / (imgwidth - 1);
      double v = double(j) / (imgheight - 1);
      ray r(origin, lower_left + u * horizontal + v * vertical - origin);

      color pixel = ray_color(r, world);
      cout << get_rgb(pixel) << '\t';
    }
    cout << "\n";
  }
  cerr << "\nDone." << endl;
}