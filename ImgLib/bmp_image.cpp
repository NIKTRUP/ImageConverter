#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;
/**
    Формат BMP похож на PPM, но имеет более сложный заголовок. Некоторые вариации поддерживают HDR и сжатие.
    Наша библиотека будет работать только с изображениями простого формата — BGR24 без сжатия.
*/

namespace img_lib {

    //Часть 1. Заголовок файла, 14 байт.
    PACKED_STRUCT_BEGIN BitmapFileHeader {
        char char_b = 'B'; char char_m = 'M'; // Подпись. Символы BM.
        uint32_t size = 0; // Суммарный размер заголовка и данных. Размер данных определяется как отступ, умноженный на высоту изображения.
        uint32_t reserved_space = 0; // Зарезервированное пространство — 4 байта, заполненные нулями.
        uint32_t indent = 54; // Отступ данных от начала файла. Он равен размеру двух частей заголовка.
    }
    PACKED_STRUCT_END

    //Часть 2. Информационный заголовок, 40 байт.
    PACKED_STRUCT_BEGIN BitmapInfoHeader {
        uint32_t size = 40; // Размер заголовка. Учитывается только размер второй части заголовка.
        int32_t width = 0;  // Ширина изображения в пикселях.
        int32_t height = 0; // Высота изображения в пикселях.
        uint16_t layers = 1; // Количество плоскостей. В нашем случае всегда одна RGB плоскость.
        uint16_t bites = 24; // Количество бит на пиксель, беззнаковое целое. В нашем случае всегда 24.
        uint32_t compression = 0; // Тип сжатия. В нашем случае всегда 0 — отсутствие сжатия.
        uint32_t indent = 0; // Количество байт в данных. Произведение отступа на высоту.
        int32_t v_resolution = 11811; // Горизонтальное разрешение, пикселей на метр. 11811 примерно соответствует 300 DPI.
        int32_t h_resolution = 11811; // Вертикальное разрешение, пикселей на метр. 11811 примерно соответствует 300 DPI.
        int32_t used_colors = 0; // Количество использованных цветов. 0 — значение не определено.
        int32_t colors = 0x1000000; // Количество значимых цветов.
    }
    PACKED_STRUCT_END

    // деление, а затем умножение на 4 округляет до числа, кратного четырём
    // прибавление тройки гарантирует, что округление будет вверх
    static int GetBMPStride(int w) {
        return 4 * ((w * 3 + 3) / 4);
    }

    bool SaveBMP(const Path& file, const Image& image) {
        ofstream out(file, ios::binary);
        const int stride = GetBMPStride( image.GetWidth() );
        BitmapFileHeader bfh;
        BitmapInfoHeader bih;
        bfh.size = image.GetHeight() * stride + bfh.indent;
        bih.width = image.GetWidth();
        bih.height = image.GetHeight();
        bih.indent = image.GetHeight() * stride;

        out.write(reinterpret_cast<const char*>(&bfh), 14);
        out.write(reinterpret_cast<const char*>(&bih), 40);

        std::vector<char> buffer(stride);
        for (int y = image.GetHeight() - 1; y >= 0; --y) {
            const Color* line = image.GetLine(y);
            for (int x = 0; x < image.GetWidth(); ++x) {
                buffer[x * 3 + 0] = static_cast<char>(line[x].b);
                buffer[x * 3 + 1] = static_cast<char>(line[x].g);
                buffer[x * 3 + 2] = static_cast<char>(line[x].r);
            }
            out.write(buffer.data(), stride);
        }
        return out.good();
    }

    Image LoadBMP(const Path& file) {
        ifstream input(file, ios::binary);

        BitmapFileHeader bfh;
        BitmapInfoHeader bih;

        input.read(reinterpret_cast<char*>(&bfh), sizeof(BitmapFileHeader));
        input.read(reinterpret_cast<char*>(&bih), sizeof(BitmapInfoHeader));
        const int stride = GetBMPStride(bih.width);

        std::vector<char> buffer(stride);

        Image img(bih.width, bih.height, Color::Black());
        for (int y = bih.height - 1; y >= 0; --y) {
            input.read(buffer.data(), stride);
            Color* line = img.GetLine(y);

            for (int x = 0; x < bih.width; ++x) {
                line[x].b = static_cast<byte>(buffer[x * 3 + 0]);
                line[x].g = static_cast<byte>(buffer[x * 3 + 1]);
                line[x].r = static_cast<byte>(buffer[x * 3 + 2]);
            }
        }
        return img;
    }

}  // namespace img_lib