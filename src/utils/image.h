#pragma once
#include <vector>
#include <cstdint>
#include <string>

struct Image {
    int width;
    int height;
    int num_channels;
    std::vector<uint8_t> data; 
};

Image load_image(const std::string& filename, int num_channels); 
void save_image(const std::string& filename, const Image& image);