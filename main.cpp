#include <fstream>
#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "raytracer.h"
#include "sphere.h"

#define MAX_DEPTH 25

color ray_color(const ray& r, const hittable& world, int depth) {
  hit rec;

  if (depth <= 0) {
    return color(0, 0, 0);
  }

  if (world.is_hit(r, 0.001, infinity, rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }
    return color(0, 0, 0);
  }

  vec3 unit_dir = unit(r.direction());
  double t = 0.5 * (unit_dir.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // Image dimensions
  const double aspect_ratio = 16.0 / 9.0;
  const int imgwidth = 400;
  const int imgheight = static_cast<int>(imgwidth / aspect_ratio);
  const int samples_per_pixel = 100;

  // World
  hittable_list world;

  auto material_ground = make_shared<diffuse>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<diffuse>(color(0.5, 0.3, 1.0));
  auto material_left = make_shared<refractor>(1.5);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

  world.add(make_shared<sphere>(pt3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(pt3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(make_shared<sphere>(pt3(-1.0, 0.0, -1.0), -0.45, material_left));
  world.add(make_shared<sphere>(pt3(1.0, 0.0, -1.0), 0.5, material_right));

  camera cam(pt3(-2,2,1), pt3(0,0,-1), vec3(0,1,0), 20, aspect_ratio);

  // Render
  ofstream image;
  image.open("example.ppm");
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
        double u = (double(i) + random_double()) / (imgwidth - 1);
        double v = (double(j) + random_double()) / (imgheight - 1);
        ray r = cam.get_ray(u, v);
        pixel += ray_color(r, world, MAX_DEPTH);
      }
      write_color(image, pixel, samples_per_pixel);
    }
  }
  cerr << "\nDone." << endl;
}