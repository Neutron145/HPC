#include <bilateral.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>

constexpr int pad_size = 1;


Image addPadding(const Image& image, int pad_size) {
    Image padded_image;
    padded_image.width = image.width + 2 * pad_size;
    padded_image.height = image.height + 2 * pad_size;
    padded_image.num_channels = image.num_channels;
    padded_image.data.resize(padded_image.width * padded_image.height * padded_image.num_channels, 0);

    for (int c = 0; c < image.num_channels; c++) {
        int channel_offset = c * padded_image.width * padded_image.height;
        
        for (int px = 0; px < padded_image.width; px++) {
            int src_x = std::clamp(px - pad_size, 0, image.width - 1);
            
            for (int py = 0; py < pad_size; py++) {
                padded_image.data[channel_offset + py * padded_image.width + px] =
                    image.data[c * image.width * image.height + 0 * image.width + src_x];
            }
            
            for (int py = image.height + pad_size; py < padded_image.height; py++) {
                padded_image.data[channel_offset + py * padded_image.width + px] =
                    image.data[c * image.width * image.height + (image.height-1) * image.width + src_x];
            }
        }
        
        for (int py = pad_size; py < image.height + pad_size; py++) {
            int src_y = py - pad_size;
            
            for (int px = 0; px < pad_size; px++) {
                padded_image.data[channel_offset + py * padded_image.width + px] =
                    image.data[c * image.width * image.height + src_y * image.width + 0];
            }
            
            for (int px = image.width + pad_size; px < padded_image.width; px++) {
                padded_image.data[channel_offset + py * padded_image.width + px] =
                    image.data[c * image.width * image.height + src_y * image.width + (image.width-1)];
            }
        }
        for (int y = 0; y < image.height; y++) {
            for (int x = 0; x < image.width; x++) {
                int src = c * image.width * image.height + y * image.width + x;
                int dst = channel_offset + (y + pad_size) * padded_image.width + (x + pad_size);
                padded_image.data[dst] = image.data[src];
            }
        }
    }

    return padded_image;
}


Bilateral::Bilateral() { }


void Bilateral::whoami() {
    std::cout << "\n\nIt is lab #3: Bilateral\n";
}


void Bilateral::setup() {
    Image image = load_image(this->filename.string(), 3);
    this->padded_image = addPadding(image, pad_size);
    this->sigma_range = 300.0f;
    this->sigma_spatial = 1.0f;
}


double Bilateral::runCPU() {
    auto start = std::chrono::high_resolution_clock::now();

    this->cpu_result.width = this->padded_image.width - 2 * pad_size;
    this->cpu_result.height = this->padded_image.height - 2 * pad_size;
    this->cpu_result.num_channels = this->padded_image.num_channels;
    this->cpu_result.data.resize(this->cpu_result.width * this->cpu_result.height * this->cpu_result.num_channels, 0);
    
    Image original_image = this->padded_image;

    for (int c = 0; c < this->cpu_result.num_channels; c++) {
        for (int x = 0; x < this->cpu_result.width; x++) {
            for (int y = 0; y < this->cpu_result.height; y++) {
                float k = 0.0f;
                float h = 0.0f;
                for (int j = -pad_size; j <= pad_size; j++) {
                    for (int i = -pad_size; i <= pad_size; i++) {
                        float g = exp(-((pow(i, 2) + pow(j, 2))/pow(this->sigma_spatial, 2)));
                        float f_ai = original_image.data[c * original_image.width * original_image.height + (y + pad_size + i) * original_image.width + (x + pad_size + j)];
                        float f_a0 = original_image.data[c * original_image.width * original_image.height + (y + pad_size) * original_image.width + (x + pad_size)];

                        float r = exp(-pow(f_ai - f_a0, 2) / pow(this->sigma_range, 2));
                        k += g * r;
                        h += g * r * f_ai;
                    }
                }
                this->cpu_result.data[c * this->cpu_result.width * this->cpu_result.height + (y) * this->cpu_result.width + (x)] = h / k;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::filesystem::path out_path = std::filesystem::path("data/bilateral_images") 
                                / (this->filename.stem().string() + "_CPU.bmp");

    save_image(out_path.string(), this->cpu_result);
    return duration.count();
}


double Bilateral::runGPU() {
    return 0.0f;
}


void Bilateral::runExperiment() {
    this->setup();
    double cpu_time = this->runCPU();

    std::cout << "CPU time: " << std::to_string(cpu_time) << " seconds\n";
}