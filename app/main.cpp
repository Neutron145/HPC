#include <iostream>
#include <IModule.h>
#include <matmul.h>
#include <memory>

using namespace std;

int main() {
    std::unique_ptr<IModule> currentLab = std::make_unique<MatMul<int>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<MatMul<float>>();
    currentLab->whoami();
    currentLab->runExperiment();

    currentLab = std::make_unique<MatMul<double>>();
    currentLab->whoami();
    currentLab->runExperiment();

    return 0;
}
