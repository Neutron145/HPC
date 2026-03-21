#pragma once
#include <IModule.h>
#include <vector>

template <typename T>
class VecSum : public IModule {
private:
    size_t size;
    std::vector<T> vec;
    T host_result, device_result;
    
    void setup() override;
    double runCPU() override;
    double runGPU() override;
public:
    VecSum();

    void whoami() override;
    void runExperiment() override;
};