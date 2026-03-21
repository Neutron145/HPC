#include <iostream>
#include <memory>
#include <filesystem>

#include <IModule.h>
#include <matmul.h>
#include <vecsum.h>
#include <spectogram.h>

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
    if (auto lab = dynamic_cast<Spectogram*>(currentLab.get())) {
        lab->filename = files[0];
    }
    std::cout << files[0] << '\n';
    currentLab->runExperiment();

    /*
    for (int i = 0; i < files.size(); i++) {
        try{
            std::cout << files[i] << '\n';
            Audio audio = loadWav(files[i]);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    */


}

int main() {
    lab3();
    return 0;
}
