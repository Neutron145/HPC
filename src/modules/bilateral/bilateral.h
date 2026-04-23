#pragma once
#include <filesystem>
#include <IModule.h>
#include <vector>
#include <image.h>


class Bilateral : public IModule {
private:

    //!< Размеры изображения
    size_t width, height;
    //!< Параметры фильтра
    float sigma_spatial, sigma_range;
    //!< Само изображение
    Image padded_image;
    //!< Результат на CPU
    Image cpu_result;
    //!< Результат на GPU
    Image gpu_result;

    void setup() override;
    double runCPU() override;
    double runGPU() override;
public:
    std::filesystem::path filename;

    Bilateral();

    void whoami() override;
    void runExperiment() override;
};