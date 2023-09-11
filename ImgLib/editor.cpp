#include "editor.h"

namespace editor {

    using namespace std;

    void HMirrInplace(img_lib::Image& image){
        for (int y = 0; y < image.GetHeight(); ++y) {
            img_lib::Color* line = image.GetLine(y);
            for (int x = 0; x < image.GetWidth()/2.0; ++x) {
                std::swap(line[x].r, line[image.GetWidth() - 1 - x].r);
                std::swap(line[x].g, line[image.GetWidth() - 1 - x].g);
                std::swap(line[x].b, line[image.GetWidth() - 1 - x].b);
            }
        }
    }

    void VMirrInplace(img_lib::Image& image){
        for (int y = 0; y < image.GetHeight()/2; ++y) {
            img_lib::Color* line_top = image.GetLine(y);
            img_lib::Color* line_bottom = image.GetLine(image.GetHeight() - 1 - y);
            std::swap_ranges(line_top, line_top + image.GetWidth(), line_bottom);
        }
    }


    void NegateInplace(img_lib::Image& image) {
        for (int y = 0; y < image.GetHeight(); ++y) {
            auto line = image.GetLine(y);
            for (int x = 0; x < image.GetWidth(); ++x) {
                line[x].r = byte(255 - std::to_integer<int>(line[x].r));
                line[x].g = byte(255 - std::to_integer<int>(line[x].g));
                line[x].b = byte(255 - std::to_integer<int>(line[x].b));
            }
        }
    }

    void ChangeBrightness(img_lib::Image& image, int brightness) {
        for (int y = 0; y < image.GetHeight(); ++y) {
            auto line = image.GetLine(y);
            for (int x = 0; x < image.GetWidth(); ++x) {
                line[x].r = byte(detail::truncate(std::to_integer<int>(line[x].r) + brightness));
                line[x].g = byte(detail::truncate(std::to_integer<int>(line[x].g) + brightness));
                line[x].b = byte(detail::truncate(std::to_integer<int>(line[x].b) + brightness));
            }
        }
    }

    img_lib::Image Sobel(const img_lib::Image& image) {
        using namespace detail;
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

    void CypherInplace(std::size_t seed, img_lib::Image& image) {
        mt19937 generator(seed);
        const int area = image.GetWidth() * image.GetHeight();

        // координаты пикселя по индексу
        auto px_num_to_coordinates = [&image](int num) {
          return std::pair{num % image.GetWidth(), num / image.GetWidth()};
        };

        // сложим индексы половины пикселей в вектор и перемешаем их
        std::vector<int> positions(area / 2);
        std::iota(positions.begin(), positions.end(), 0);
        Shuffle(positions.begin(), positions.end(), generator);

        // Будем переставлять чётные и нечётные пиксели между собой.
        // Благодаря этому процедура шифрования и дешифрования будет одинаковой.
        for (int px_num = 0; px_num < area/2; ++px_num) {
            auto [src_x, src_y] = px_num_to_coordinates(px_num * 2);
            auto [dst_x, dst_y] = px_num_to_coordinates(positions[px_num] * 2 + 1);

            std::swap(image.GetPixel(src_x, src_y), image.GetPixel(dst_x, dst_y));
        }
    }

    img_lib::Image CropInplace(const img_lib::Image& image, Point left_top, Point right_down) {
        bool check_order = left_top.x < right_down.x && left_top.y < right_down.y ,
        check_pos = (image.GetHeight() > left_top.y &&  image.GetHeight() > right_down.y) &&
                    (image.GetWidth() > left_top.x &&  image.GetWidth() > right_down.x);
        if(!check_order || !check_pos){
            throw std::runtime_error("Error: croping failed ");
        }

        img_lib::Image result_image(right_down.x - left_top.x , right_down.y - left_top.y, img_lib::Color::Black());

        for (int y = 0; y < result_image.GetHeight(); ++y) {
            auto line = image.GetLine(y + left_top.y);
            auto result_line = result_image.GetLine(y);
            for (int x = 0; x < result_image.GetWidth(); ++x) {
                result_line[x].r = line[x + left_top.x].r;
                result_line[x].g = line[x + left_top.x].g;
                result_line[x].b = line[x + left_top.x].b;
            }
        }

        return result_image;
    }
}
