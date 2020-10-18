#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

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

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<diffuse>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      pt3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - pt3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<diffuse>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<refractor>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<refractor>(1.5);
  world.add(make_shared<sphere>(pt3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<diffuse>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(pt3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(pt3(4, 1, 0), 1.0, material3));

  return world;
}

hittable_list test() {
  hittable_list world;

  auto ground_material = make_shared<diffuse>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(pt3(0, -1000, 0), 1000, ground_material));

  auto material1 = make_shared<refractor>(1.5);
  world.add(make_shared<sphere>(pt3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<diffuse>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(pt3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(pt3(4, 1, 0), 1.0, material3));

  return world;
}

void raytrace(int* startX, int* startY, int tilewidth, int tileheight,
              int imgwidth, int imgheight, int spp, hittable_list world,
              camera cam, mutex* tileMutex, color* buffer) {
  while (true) {
    tileMutex->lock();
    int tileX = *startX;
    int tileY = *startY;

    *startX += tilewidth;
    if (*startX >= imgwidth) {
      *startX = 0;
      *startY += tileheight;
    }
    cerr << "\r" << "Starting tile: (" << tileX << ", " << tileY << ")" << flush;
    tileMutex->unlock();

    if (tileY >= imgheight) {
      return;
    }

    tilewidth = tilewidth - max(0, tileX + tilewidth - imgwidth);
    tileheight = tileheight - max(0, tileY + tileheight - imgheight);

    for (int j = tileY + tileheight - 1; j >= tileY; j--) {
      // cerr << "\r" << j << " lines remaining." << flush;
      for (int i = tileX; i < tileX + tilewidth; i++) {
        color pixel(0, 0, 0);
        for (int s = 0; s < spp; s++) {
          double u = (double(i) + random_double()) / (imgwidth - 1);
          double v = (double(j) + random_double()) / (imgheight - 1);
          ray r = cam.get_ray(u, v);
          pixel += ray_color(r, world, MAX_DEPTH);
        }
        // write_color(image, pixel, samples_per_pixel);

        int bufInd = (i + j * imgwidth);
        buffer[bufInd] = pixel;
      }
    }
  }
}

int main() {
  // Image
  const auto aspect_ratio = 3.0 / 2.0;
  const int imgwidth = 1200;
  const int imgheight = static_cast<int>(imgwidth / aspect_ratio);
  const int samples_per_pixel = 10;
  const int max_depth = 50;

  // World
  auto world = random_scene();

  // Camera
  pt3 lookfrom(13, 2, 3);
  pt3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

  // Thread Info
  color* buffer = new color[imgwidth * imgheight];
  int tileWidth = 32;
  int tileHeight = 32;
  int startX = 0;
  int startY = 0;
  mutex tileMutex;
  unsigned int numThreads = max(1, (int)thread::hardware_concurrency());

  std::thread* threads = new std::thread[numThreads];

  for (int i = 0; i < numThreads; i++) {
    threads[i] = thread(raytrace, &startX, &startY, tileWidth, tileHeight, imgwidth, imgheight, samples_per_pixel, world, cam, &tileMutex, buffer);
  }

  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }

  // Render Output
  ofstream image;
  image.open("example.ppm");
  image << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";
  for (int j = imgheight - 1; j >=0; j--) {
    cerr << "\r" << j << " lines remaining." << flush;
    for (int i = 0; i < imgwidth; i++) {
      color pixel = buffer[i + (j * imgwidth)]; 
      write_color(image, pixel, samples_per_pixel);
    }
  }

  cerr << "\nDone." << endl;
}

// TODO - clean up snake, camel, case structure