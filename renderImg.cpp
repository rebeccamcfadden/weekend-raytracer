#include <iostream>

#include "color.h"
#include "vec3.h"

using namespace std;

int main() {
    int imgwidth = 256;
    int imgheight = 256;

    cout << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";

    for (int i = 0; i < imgheight; i++) {
        cerr << "\r" << (imgheight - i) << " lines remaining." << flush;
        for (int j = 0; j < imgwidth; j++) {
            color pixel(double(i) / (imgheight-1), 0.25, double(j) / (imgwidth - 1));
            cout << get_rgb(pixel) << '\t';
        }
        cout << "\n";
    }
    cerr << "\nDone." << endl;
}