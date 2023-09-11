#include <bmp_image.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <iostream>
#include <string_view>
#include <unordered_map>

#include <editor.h>

using namespace std;

enum class Format { JPEG, PPM, BMP, UNKNOWN };
enum class Actions {HMIRROR = 1, VMIRROR, SOBEL, NEGATE, CYPHER, BRIGHTNESS, CROP};
enum class Errors { UsageFailed = 1, UnknowInFormat, UnknowOutFormat, LoadingFailed, SavingFailed, UsedOptions };

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }
    if (ext == ".ppm"sv) {
        return Format::PPM;
    }
    if (ext == ".bmp"sv) {
        return Format::BMP;
    }

    return Format::UNKNOWN;
}

class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
};

namespace format_interfaces {

    class PPM : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SavePPM(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadPPM(file);
        }
    };

    class JPEG : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveJPEG(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadJPEG(file);
        }
    };

    class BMP : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveBMP(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadBMP(file);
        }
    };

}  // namespace format_interfaces

const ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    Format format = GetFormatByExtension(path);
    if (format == Format::UNKNOWN) {
        return nullptr;
    }

    switch (format) {
        case Format::PPM:
            static const format_interfaces::PPM ppmInterface;
            return &ppmInterface;

        case Format::JPEG:
            static const format_interfaces::JPEG jpegInterface;
            return &jpegInterface;

        case Format::BMP:
            static const format_interfaces::BMP bmpInterface;
            return &bmpInterface;
    }

    return nullptr;
}

int TryApplyCommand(img_lib::Image& image, int argc, const char** argv){
    std::string action = argv[3];

    static unordered_map<std::string, int> actions = {{"hmirror", static_cast<int>(Actions::HMIRROR)},
                                                      {"vmirror", static_cast<int>(Actions::VMIRROR)},
                                                      {"sobel", static_cast<int>(Actions::SOBEL)},
                                                      {"negate", static_cast<int>(Actions::NEGATE)},
                                                      {"cypher", static_cast<int>(Actions::CYPHER)},
                                                      {"brightness", static_cast<int>(Actions::BRIGHTNESS)},
                                                      {"crop", static_cast<int>(Actions::CROP)}
                                                     };
    if(actions.count(action) == 0){
        return static_cast<int>(Errors::UsedOptions);
    }

    switch (actions.at(action)) {
        case static_cast<int>(Actions::HMIRROR):{
            editor::HMirrInplace(image);
            break;
        }
        case static_cast<int>(Actions::VMIRROR):{
            editor::VMirrInplace(image);
            break;
        }
        case static_cast<int>(Actions::SOBEL):{
            image = editor::Sobel(image);
            break;
        }
        case static_cast<int>(Actions::NEGATE):{
            editor::NegateInplace(image);
            break;
        }
        case static_cast<int>(Actions::CYPHER):{
            if(argc == 5){
                std::size_t key = static_cast<std::size_t>(atoi(argv[4]));
                editor::CypherInplace(key, image);
                break;
            }else{
                return static_cast<int>(Errors::UsedOptions);
            }
        }
        case static_cast<int>(Actions::BRIGHTNESS):{
            if(argc == 5){
                int brightness = atoi(argv[4]);
                editor::ChangeBrightness(image, brightness);
                break;
            }else {
                return static_cast<int>(Errors::UsedOptions);
            }
        }
        case static_cast<int>(Actions::CROP):{
            if(argc == 8){
                editor::Point   left_top = {static_cast<std::size_t>(atoi(argv[4])), static_cast<std::size_t>(atoi(argv[5]))},
                    right_down = {static_cast<std::size_t>(atoi(argv[6])), static_cast<std::size_t>(atoi(argv[7]))};
                try {
                    image = editor::CropInplace(image, left_top, right_down);
                } catch (const std::runtime_error& er){
                    std::cerr << er.what();
                    return static_cast<int>(Errors::UsedOptions);
                }
                break;
            } else {
                return static_cast<int>(Errors::UsedOptions);
            }
        }
    }
    return 0;

//    if(action == "hmirror"sv) {
//        editor::HMirrInplace(image);
//    }else if(action == "vmirror"sv) {
//        editor::VMirrInplace(image);
//    }else if(action == "sobel"sv) {
//        image = editor::Sobel(image);
//    }else if(action == "negate"sv) {
//        editor::NegateInplace(image);
//    }else if(action == "cypher"sv && argc == 5) {
//        int key = atoi(argv[4]);
//        editor::CypherInplace(key, image);
//    }else if(action == "brightness"sv && argc == 5){
//        int brightness = atoi(argv[4]);
//        editor::ChangeBrightness(image, brightness);
//    }else if(action == "crop"sv && argc == 8){
//        editor::Point   left_top = {atoi(argv[4]), atoi(argv[5])},
//            right_down = {atoi(argv[6]), atoi(argv[7])};
//        try {
//            image = editor::CropInplace(image, left_top, right_down);
//            std::runtime_error
//        } catch (const std::runtime_error& er){
//            std::cerr << er.what();
//            return static_cast<int>(Errors::UsedOptions);
//        }
//    }
}

int main(int argc, const char** argv) {
    if (argc < 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return static_cast<int>(Errors::UsageFailed);
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    const ImageFormatInterface* in_format_interface = GetFormatInterface(in_path);
    if (!in_format_interface) {
        cerr << "Unknown format of the input file"sv << endl;
        return static_cast<int>(Errors::UnknowInFormat);
    }

    const ImageFormatInterface* out_format_interface = GetFormatInterface(out_path);
    if (!out_format_interface) {
        cerr << "Unknown format of the output file"sv << endl;
        return static_cast<int>(Errors::UnknowOutFormat);
    }

    img_lib::Image image = in_format_interface->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return static_cast<int>(Errors::LoadingFailed);
    }

    if(argc > 3 && (TryApplyCommand(image, argc, argv) == static_cast<int>(Errors::UsedOptions))) {
        cerr << "Option is wrong. Usage: [hmirror|vmirror|sobel|negate|cypher <key>|brightness <value>|crop <lt_x> <lt_y> <rd_x> <rd_y>]"sv << endl;
        return static_cast<int>(Errors::UsedOptions);
    }

    if (!out_format_interface->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return static_cast<int>(Errors::SavingFailed);
    }

    cout << "Successfully converted"sv << endl;
}