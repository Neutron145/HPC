#include <image.h>

void saveImage(std::vector<std::vector<float>> results, std::string filename) {
    BMP image;
    image.SetSize(results.size(), results[0].size());

    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results[0].size(); j++) {
            RGBApixel pixel;
            pixel.Red = results[i][j];
            pixel.Green = results[i][j];
            pixel.Blue = results[i][j];
            pixel.Alpha = 0;
            image.SetPixel(i, j, pixel);
        }
    }
    
    image.WriteToFile(filename);
}