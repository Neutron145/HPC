#include <iostream>
#include <iomanip>
#include <chrono>
#include <span>
#include <vector>
#include <cmath>
#include <numbers>
#include <algorithm>

#include <spectogram.h>
#include <fftw3.h>
#include <EasyBMP.h>1


std::vector<float> generateSine(float freq, float sample_rate, int n_samples) {
    std::vector<float> samples(n_samples);
    for (int i = 0; i < n_samples; i++) {
        samples[i] = std::sin(2.0f * std::numbers::pi * freq * i / sample_rate);
    }
    return samples;
}


std::vector<std::span<float>> makeChunks(std::span<float> samples, 
                                    std::size_t window_size,
                                    std::size_t hop_size) {
    std::vector<std::span<float>> result;
    
    for (std::size_t i = 0; i + window_size <= samples.size(); i += hop_size) {
        std::span<float> chunk{samples.data() + i, window_size};
        result.push_back(chunk);
    }

    return result;
}


RGBApixel colorMap(float x, float max, float min) {
    RGBApixel pixel;
    float value = (x - min) / (max - min);

    if (value < 0.33) {
        float local_value = value / 0.33;
        pixel.Red = 255 * local_value;
        pixel.Green = 0;
        pixel.Blue = 0;
    }
    else if (value < 0.66) {
        float local_value = (value - 0.33) / 0.33;
        pixel.Red = 255;
        pixel.Green = 255 * local_value;
        pixel.Blue = 0;
    }
    else {
        float local_value = (value - 0.66) / 0.33;
        pixel.Red = 255;
        pixel.Green = 255;
        pixel.Blue = 255 * local_value;
    }

    pixel.Alpha = 0;
    return pixel;
}


void saveImage(std::vector<std::vector<float>>& results, std::string filename) {
    BMP image;
    image.SetSize(results.size(), results[0].size());
    
    //Нормализация
    std::vector<float> local_maxs{};
    std::vector<float> local_mins{};
    for (int i = 0; i < results.size(); i++) {
        local_mins.push_back(*std::ranges::min_element(results[i]));
        local_maxs.push_back(*std::ranges::max_element(results[i]));
    }
    float min = *std::ranges::min_element(local_mins);
    float max = *std::ranges::max_element(local_maxs);

    std::cout << "min dB: " << min << "\n";
    std::cout << "max dB: " << max << "\n";


    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results[0].size(); j++) {
            image.SetPixel(i, results[0].size() - j - 1, colorMap(results[i][j], max, min));
        }
    }
    
    image.WriteToFile(filename.c_str());
}

Spectogram::Spectogram() { }


void Spectogram::whoami() {
    std::cout << "\n\nIt is lab #2: Spectogram\n";
}


void Spectogram::setup() {
    this->signal = loadWav(this->filename);
    //this->signal.samples = generateSine(1000, 44100, 44100*5);
}


double Spectogram::runGPU() {
    return 0;
}


double Spectogram::runCPU() {
    std::vector<std::span<float>> chunks = makeChunks(this->signal.samples, 512, 256);
    std::vector<std::vector<float>> results_CPU;

    int out_size = (chunks[0].size() / 2) + 1;
    float* fftw_in = fftwf_alloc_real(chunks[0].size());
    fftwf_complex* fftw_out = fftwf_alloc_complex(out_size);
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(chunks[0].size(), fftw_in, fftw_out, FFTW_ESTIMATE); 

    for (int i = 0; i < chunks.size(); i++) {
        std::ranges::copy(chunks[i], fftw_in);

        fftwf_execute(plan);
        
        std::vector <float> result;
        for (int j = 0; j < out_size; j++) {
            result.push_back(fftw_out[j][0] * fftw_out[j][0] + fftw_out[j][1] * fftw_out[j][1]);
        }
        
        std::ranges::for_each(result, [](auto& v) {
            v = 10.0 * std::log10(v + 1e-8);
        });

        results_CPU.push_back(result);
    }  

    fftwf_destroy_plan(plan);
    fftwf_free(fftw_in);
    fftwf_free(fftw_out);
    std::cout << "samples size: " << this->signal.samples.size() << "\n";
    saveImage(results_CPU, "test.bmp");

    return 0;
}  

void Spectogram::runExperiment() {
    this->setup();

    double cpu_time = this->runCPU();
    double gpu_time = this->runGPU();
}
