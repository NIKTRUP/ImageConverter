#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>
#include <cassert>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

// TODO:: Реализовать свёртку и класс ядра.

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image& image) {
    img_lib::Image result_image(image.GetWidth(), image.GetHeight(), img_lib::Color::Black());

    for (int y = 1; y < (image.GetHeight() - 1); ++y) {
        for (int x = 1; x < (image.GetWidth() - 1); ++x) {
            int gx = -Sum(image.GetPixel(x - 1, y - 1)) - (2 * Sum(image.GetPixel(x, y - 1))) - Sum(image.GetPixel(x + 1, y - 1)) +
                    Sum(image.GetPixel(x - 1, y + 1)) + (2 * Sum(image.GetPixel(x, y + 1))) + Sum(image.GetPixel(x + 1, y + 1));
            int gy = -Sum(image.GetPixel(x - 1, y - 1)) - (2 * Sum(image.GetPixel(x - 1, y))) - Sum(image.GetPixel(x - 1, y + 1)) +
                    Sum(image.GetPixel(x + 1, y - 1)) + (2 * Sum(image.GetPixel(x + 1, y))) + Sum(image.GetPixel(x + 1, y + 1));

            double value = std::clamp<double>(static_cast<double>(sqrt((gx * gx) + (gy * gy))), 0, 255);
            img_lib::Color* color = &result_image.GetPixel(x, y);
            color->r = static_cast<byte>(value);
            color->g = static_cast<byte>(value);
            color->b = static_cast<byte>(value);
        }
    }

    return result_image;
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}