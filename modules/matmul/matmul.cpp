#include <matmul.h>
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <type_traits>

template <typename T>
extern float launchMatMulKernel(const T* A, const T* B, T* C, int M, int K, int N);

template <typename T>
MatMul<T>::MatMul() { }

template <typename T>
void MatMul<T>::whoami() {
    std::string type_name {"Unknown"};
    
    if constexpr (std::is_same_v<T, int>) {
        type_name = "int";
    } else if constexpr (std::is_same_v<T, float>) {
        type_name = "float";
    } else if constexpr (std::is_same_v<T, double>) {
        type_name = "double";
    }

    std::cout << "\n\nIt is lab #0: MatMul\n";
    std::cout << "Used type: " << type_name << "\n";
}

template <typename T>
double MatMul<T>::verifyResult() {
    double max_error { 0.0 };
    size_t size {this->M * this->N};

    for (size_t i = 0; i < size; i++) {
        double diff = std::abs(static_cast<double>(this->host_C[i]) - 
                               static_cast<double>(this->device_C[i]));
        max_error = std::max(diff, max_error);
    }

    return max_error;
}

template <typename T>
void MatMul<T>::setup() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis(-10, 11);
    
    A.resize(this->M * this->K);
    B.resize(this->K * this->N);
    host_C.resize(this->M * this->N);
    device_C.resize(this->M * this->N);

    for (size_t i = 0; i < this->M * K; i++) {
        A[i] = static_cast<T>(dis(gen));
    }
    for (size_t i = 0; i < this->M * K; i++) {
        A[i] = static_cast<T>(dis(gen));
    }
}

template <typename T>
double MatMul<T>::runCPU() {
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < this->M; i++) {
        for (size_t j = 0; j < this->N; j++) {
            T sum = 0;
            for (size_t k = 0; k < this->K; k++) {
                sum += this->A[i * this->K + k] * this->B[k * this->N + j];
            }
            this->host_C[i * this->N + j] = sum;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

template <typename T>
double MatMul<T>::runGPU() {
    float time = launchMatMulKernel<T>(this->A.data(), this->B.data(), this->device_C.data(), this->M, this->K, this->N);
    return time;
}

template <typename T>
void MatMul<T>::runExperiment() {
    int sizes[5] = {100, 500, 1000, 1500, 2000};
    std::cout << std::left 
                << std::setw(10) << "Size" 
                << std::setw(16) << "CPU time, ms" 
                << std::setw(16) << "GPU time, ms" 
                << std::setw(10) << "S"
                << std::setw(10) << "Max error\n";
    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < 5; i++) {
        this->M = sizes[i], this->K = sizes[i], this->N = sizes[i];
        this->setup();
        double cpu_time = this->runCPU();
        double gpu_time = this->runGPU();
        double max_error = verifyResult();
        
        std::cout << std::left
                    << std::setw(10) << sizes[i] 
                    << std::setw(16) << cpu_time
                    << std::setw(16) << gpu_time
                    << std::setw(10) << cpu_time / gpu_time
                    << std::setw(10) << max_error << "\n";
    }
}

template class MatMul<float>;
template class MatMul<double>;
template class MatMul<int>;
template class MatMul<long>;
template class MatMul<long long>;