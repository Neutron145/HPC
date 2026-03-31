#include <cuda_runtime.h>
#include <cufft.h>
#include <vector>
#include <chrono>
#include <iostream>


__global__ void spectogramKernel(cufftComplex* d_in, float* d_out, int out_size, int batches) {
    unsigned int idx = blockDim.x * blockIdx.x + threadIdx.x;
    if (idx < batches * out_size) {
        d_out[idx] = 10 * log10f(d_in[idx].x * d_in[idx].x + d_in[idx].y * d_in[idx].y + 1e-8f);
    }
}


float launchSpectogramKernel(const float* data, int size, int window_size, int hop_size, std::vector<std::vector<float>>& result) {
    int batches = ((size - window_size) / hop_size) + 1;
    int out_size = window_size / 2 + 1;
    
    float* d_in;            //!< Входной массив на девайсе
    float* d_result;        //!< Результат на девайсе
    float *h_result;        //!< Результат на хосте
    cufftComplex* d_out;    //!< Результат FFT
    
    //!< Преобразовываем плоский массив с перекрытиями в плоский массив без перекрытий (явное разделение)
    std::vector<float> flat(batches * window_size);
    for (int i = 0; i < batches; i++) {
        for (int j = 0; j < window_size; j++) {
            flat[i * window_size + j] = data[i * hop_size + j];
        }
    }

    //!< Выделение памяти для всех массивов
    h_result = (float*)malloc(out_size * batches * sizeof(float));
    cudaMalloc(&d_in, batches * window_size * sizeof(float));
    cudaMalloc(&d_out, out_size * batches * sizeof(cufftComplex));
    cudaMalloc(&d_result, out_size * batches * sizeof(float));
    

    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);
    cudaEventRecord(start);

    cudaMemcpy(d_in, flat.data(), batches * window_size * sizeof(float), cudaMemcpyHostToDevice);
    
    //!< Выполнение FFT для всех окон одновременно
    cufftHandle plan;
    cufftPlanMany(&plan, 1, &window_size, NULL, 1, window_size, NULL, 1, out_size, CUFFT_R2C, batches);
    cufftExecR2C(plan, d_in, d_out);

    //!< Постобработка на GPU - вычисление спектра и перевод в dB
    int threads_per_block = 256;
    int num_blocks = (batches * out_size + threads_per_block - 1) / threads_per_block;
    spectogramKernel<<<num_blocks, threads_per_block>>>(d_out, d_result, out_size, batches);

    cudaMemcpy(h_result, d_result, out_size * batches * sizeof(float), cudaMemcpyDeviceToHost);
    cudaEventRecord(end);

    //!< Перевод результата в формат класса Spectogram
    auto cpu_start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < batches; i++) {
        std::vector<float> batch_result(out_size);
        for(int j = 0; j < out_size; j++) {
            batch_result[j] = h_result[i * out_size + j];
        }
        result.push_back(batch_result);
    }
    auto cpu_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpu_milliseconds = cpu_end - cpu_start;


    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);

    cudaFree(d_in);
    cudaFree(d_out);
    free(h_result);

    return milliseconds + cpu_milliseconds.count();
}