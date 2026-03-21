#include <vecsum.h>
#include <iostream>
#include <iomanip>
#include <chrono>

template <typename T>
extern float launchVecSumKernel(const T* vec, T* res, int size);

template <typename T>
VecSum<T>::VecSum() { }

template <typename T>
void VecSum<T>::whoami() {
    std::string type_name {"Unknown"};
    
    if constexpr (std::is_same_v<T, int>) {
        type_name = "int";
    } else if constexpr (std::is_same_v<T, float>) {
        type_name = "float";
    } else if constexpr (std::is_same_v<T, double>) {
        type_name = "double";
    }

    std::cout << "It is lab #1: VecSum\n";
    std::cout << "Used type: " << type_name << "\n";
}

template <typename T>
void VecSum<T>::setup() {
    this->vec.resize(this->size);

    for (size_t i = 0; i < this->size; i++) {
        this->vec[i] = static_cast<T>(1);
    }
}

template <typename T>
double VecSum<T>::runCPU() {
    auto start = std::chrono::high_resolution_clock::now();

    this->host_result = 0;
    for (int i = 0; i < this->vec.size(); i++) {
        this->host_result += this->vec[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}


template <typename T>
double VecSum<T>::runGPU() {
    float time = launchVecSumKernel<T>(this->vec.data(), &this->device_result, this->vec.size());
    return time;
}

template <typename T>
void VecSum<T>::runExperiment() {
    int sizes[5] = {1000, 10000, 100000, 500000, 1000000};
    std::cout << std::left 
            << std::setw(10) << "Size" 
            << std::setw(16) << "CPU time, ms" 
            << std::setw(16) << "GPU time, ms" 
            << std::setw(10) << "S"
            << std::setw(16) << "Result host"
            << std::setw(16) << "Result dev" << "\n";

    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < 5; i++) {
        this->size = sizes[i];
        this->setup();

        double cpu_time = this->runCPU();
        double gpu_time = this->runGPU();

        std::cout << std::left
            << std::setw(10) << sizes[i] 
            << std::setw(16) << cpu_time
            << std::setw(16) << gpu_time
            << std::setw(10) << cpu_time / gpu_time
            << std::setw(16) << host_result
            << std::setw(16) << device_result << "\n";
    }
}

template class VecSum<float>;
template class VecSum<double>;
template class VecSum<int>;
template class VecSum<long>;
template class VecSum<long long>;