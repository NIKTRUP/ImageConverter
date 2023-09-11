#ifndef IMGLIB_IMGCONVERTER_EDITOR_H
#define IMGLIB_IMGCONVERTER_EDITOR_H

#include <stdexcept>
#include <random>
#include <numeric>
#include <cstdint>
#include <cstddef>
#include <algorithm>

#include "img_lib.h"

namespace editor {

    struct Point{
        std::size_t x = 0,
                    y = 0;
    };

    namespace detail {
        inline int truncate(int value) {
            return (value <= 0)  ? 0 : ( value >= 255 ? 255 : value );
        }

        inline int Sum(img_lib::Color c) {
            return std::to_integer<int>(c.r) + std::to_integer<int>(c.g) + std::to_integer<int>(c.b);
        }

    }

    void HMirrInplace(img_lib::Image& image);

    void VMirrInplace(img_lib::Image& image);

    void NegateInplace(img_lib::Image& image);

    void ChangeBrightness(img_lib::Image& image, int brightness);

    img_lib::Image Sobel(const img_lib::Image& image);

    // Получение равномерно распределённого числа от 0 до max - 1.
    // В отличие от uniform_int_distribution работает на всех платформах одинаково
    template <typename Engine>
    std::uint32_t GetRandomNumber(std::uint32_t max, Engine& engine) {
        std::uint32_t engine_max = static_cast<uint32_t>(engine.max());

        if (max == engine_max) {
            return static_cast<uint32_t>(engine());
        }

        while (true) {
            std::uint32_t val = static_cast<uint32_t>(engine());
            if (val / max < engine_max / max || engine_max % max == max - 1u) {
                return val % max;
            }
        }
    }

    // В отличие от std::shuffle работает на всех платформах одинаково
    template <typename RandomIt, typename Engine>
    void Shuffle(RandomIt first, RandomIt last, Engine& engine) {
        using IterDiff = typename std::iterator_traits<RandomIt>::difference_type;
        IterDiff i, n;
        n = last - first;
        for (i = n - 1; i > 0; --i) {
            std::swap(first[i], first[GetRandomNumber(static_cast<std::uint32_t>(i + 1), engine)]);
        }
    }

    void CypherInplace(std::size_t seed, img_lib::Image& image);

    img_lib::Image CropInplace(const img_lib::Image& image, Point left_top, Point right_down);
}

#endif //IMGLIB_IMGCONVERTER_EDITOR_H
