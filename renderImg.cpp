#include <iostream>
using namespace std;

int main() {
    int imgwidth = 256;
    int imgheight = 256;

    cout << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";

    for (int i = 0; i < imgheight; i++) {
        for (int j = 0; j < imgwidth; j++) {
            auto r = double(i) / (imgheight-1);
            auto g = 0.25;
            auto b = double(j) / (imgwidth-1);

            int ir = static_cast<int>(255.99 * r);
            int ig = static_cast<int>(255.99 * g);
            int ib = static_cast<int>(255.99 * b);

            cout << ir << ' ' << ig << ' ' << ib << '\t';
        }
        cout << "\n";
    }
}