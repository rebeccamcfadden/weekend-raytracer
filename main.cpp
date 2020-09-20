#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

using namespace std;

bool hit_sphere(const pt3 &center, double radius, const ray &r) {
  // GENERAL IDEA:
  // (p - center) dot (p - center) = rad^2
  // p = r.origin + t*r.direction
  // quadratic formula : x = (-b +/- sqrt(b^2 - 4ac))/2a
  // discriminant : b^2 - 4ac
  // a = r.direction dot r.direction
  // b = 2 * (r.direction dot (r.origin - center))
  // c = (r.origin - center) dot (r.origin - center) - rad ^2

  vec3 oc = r.origin() - center;  // origin - center
  double a = r.direction().dot(r.direction());
  double b = 2.0 * oc.dot(r.direction());
  double c = oc.dot(oc) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  return (discriminant > 0);
}

color ray_color(const ray &r) {
  if (hit_sphere(pt3(0, 0, -1), 1, r)) {
    return color(1, 0, 0);
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

      color pixel = ray_color(r);
      cout << get_rgb(pixel) << '\t';
    }
    cout << "\n";
  }
  cerr << "\nDone." << endl;
}