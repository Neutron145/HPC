#pragma once
#include <IModule.h>
#include <vector>

template <typename T>
class MatMul : public IModule {
private: 
    //!< Размерности матриц 
    size_t M, K, N;
    //!< Матрицы хранятся в виде одномерных массивов построчно
    std::vector<T> A, B;
    std::vector<T> host_C, device_C;

    double verifyResult();
    void setup() override;
    double runCPU() override;
    double runGPU() override;
public:
    MatMul();

    void whoami() override;
    void runExperiment() override;
};
