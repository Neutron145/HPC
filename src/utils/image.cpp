#include <image.h>
#include <EasyBMP.h>
#include <stdexcept>

Image load_image(const std::string& filename, int num_channels) {
    BMP bmp_image;
    Image image;
    
    if (!bmp_image.ReadFromFile(filename.c_str())) {
        throw std::runtime_error("Failed to load image: " + filename);
    }

    image.width = bmp_image.TellWidth();
    image.height = bmp_image.TellHeight();
    image.num_channels = num_channels;
    image.data.resize(image.width * image.height * num_channels);

    for(int i = 0; i < image.width; i++) {
        for(int j = 0; j < image.height; j++) {
            RGBApixel pixel = bmp_image.GetPixel(i, j);
            if (num_channels == 1) {
                uint8_t gray = static_cast<uint8_t>(pixel.Red);
                image.data[j * image.width + i] = gray;
            } else {
                image.data[0 * image.width * image.height + j * image.width + i] = pixel.Red;
                image.data[1 * image.width * image.height + j * image.width + i] = pixel.Green;
                image.data[2 * image.width * image.height + j * image.width + i] = pixel.Blue;
            }
        }
    }

    return image;
}

void save_image(const std::string& filename, const Image& image) {
    BMP bmp_image;
    bmp_image.SetSize(image.width, image.height);

    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            RGBApixel pixel;
            if (image.num_channels == 1) {
                uint8_t gray = image.data[j * image.width + i];
                pixel.Red = gray;
                pixel.Green = gray;
                pixel.Blue = gray;
            } else {
                pixel.Red   = image.data[0 * image.width * image.height + j * image.width + i];
                pixel.Green = image.data[1 * image.width * image.height + j * image.width + i];
                pixel.Blue  = image.data[2 * image.width * image.height + j * image.width + i];
            }
            pixel.Alpha = 0;
            bmp_image.SetPixel(i, j, pixel);
        }
    }
    bmp_image.WriteToFile(filename.c_str());
}