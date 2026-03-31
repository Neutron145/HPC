#pragma once 
#include <filesystem>
#include <IModule.h>
#include <wav.h>

class Spectogram : public IModule {
private:
    Audio signal;
    
    int window_size;
    int hop_size;

    void setup() override;
    double runCPU() override;
    double runGPU() override;

public:
    std::filesystem::path filename;

    Spectogram();

    void whoami() override;
    void runExperiment() override;
};