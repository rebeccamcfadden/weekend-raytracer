#include <iostream>
#include <fstream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
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
  const int samples_per_pixel = 100;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(pt3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(pt3(0, -100.5, -1), 100));

  camera cam;

  // Render
  ofstream image;
  image.open ("example.ppm");
  image << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";
  // cout << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";
  // u vector is x dir
  // v vector is y dir
  // to draw from top left, have to go off u = 0, v = imgheight
  for (int j = imgheight - 1; j >= 0; j--) {
    cerr << "\r" << j << " lines remaining." << flush;
    for (int i = 0; i < imgwidth; i++) {
      color pixel(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; s++) {
        double u = (i + random_double()) / (imgwidth - 1);
        double v = (j + random_double()) / (imgheight - 1);
        ray r = cam.get_ray(u, v);
        pixel += ray_color(r, world);
      }
      write_color(image, pixel, 100);
    }
  }
  cerr << "\nDone." << endl;
}