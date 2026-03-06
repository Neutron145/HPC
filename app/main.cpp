#include <iostream>
#include <IModule.h>
#include <memory>

#include <matmul.h>
#include <vecsum.h>

using namespace std;

void lab1() {
    std::unique_ptr<IModule> currentLab = std::make_unique<MatMul<int>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<MatMul<float>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<MatMul<double>>();
    currentLab->whoami();
    currentLab->runExperiment();
}

void lab2() {
    std::unique_ptr<IModule> currentLab = std::make_unique<VecSum<int>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<VecSum<float>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<VecSum<double>>();
    currentLab->whoami();
    currentLab->runExperiment();
}

int main() {
    lab2();
    return 0;
}
