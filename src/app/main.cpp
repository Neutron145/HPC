#include <iostream>
#include <memory>
#include <filesystem>

#include <IModule.h>
#include <matmul.h>
#include <vecsum.h>
#include <spectogram.h>
#include <bilateral.h>

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

void lab3() {
    std::unique_ptr<IModule> currentLab = std::make_unique<Spectogram>();
    
    std::filesystem::path directory = "./data/wav";
    std::vector<std::filesystem::path> files;

    for (const auto& file : std::filesystem::directory_iterator(directory)) {
        if(file.is_regular_file()) {
            files.push_back(file.path());
        }
    }

    currentLab->whoami();
    for (int i = 0; i < files.size(); i++) {
        if (auto lab = dynamic_cast<Spectogram*>(currentLab.get())) {
            lab->filename = files[i];
        }
        std::cout << files[i] << '\n';
        currentLab->runExperiment();
    }
}

void lab4() {
    std::unique_ptr<IModule> currentLab = std::make_unique<Bilateral>();
    
    std::filesystem::path directory = "./data/images";
    std::vector<std::filesystem::path> files;

    for (const auto& file : std::filesystem::directory_iterator(directory)) {
        if(file.is_regular_file()) {
            files.push_back(file.path());
            std::cout << file.path() << '\n';
        }
    }

    currentLab->whoami();
    if (auto lab = dynamic_cast<Bilateral*>(currentLab.get())) {
        lab->filename = files[1];
    }
    std::cout << files[1] << '\n';
    currentLab->runExperiment();
}

int main() {
    lab4();
    return 0;
}
