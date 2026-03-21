#pragma once 
#include <filesystem>
#include <IModule.h>
#include <wav.h>

class Spectogram : public IModule {
private:
    Audio signal;
    
    std::vector<std::vector<float>> results_GPU;

    void setup() override;
    double runCPU() override;
    double runGPU() override;

public:
    std::filesystem::path filename;

    Spectogram();

    void whoami() override;
    void runExperiment() override;
};