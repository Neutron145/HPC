#include <cstdio>
#include <iostream>

template <typename T>
__global__ void vecSumKernel(const T* vec, T* res, int size) {
    extern __shared__ double raw_sdata[]; 
    
    T* sdata = reinterpret_cast<T*>(raw_sdata);

    unsigned int tid = threadIdx.x;
    unsigned int idx = blockDim.x * blockIdx.x * 2 + threadIdx.x;

    T sum = 0.0f;

    if (idx < size) sum += vec[idx];
    if (idx + blockDim.x < size) sum += vec[idx + blockDim.x];

    sdata[tid] = sum;
    __syncthreads();

    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    if (tid == 0) {
        res[blockIdx.x] = sdata[0];
    }
}

template <typename T>
float launchVecSumKernel(const T* vec, T* res, int size) {
    T *d_vec, *d_res;
    
    int threads = 128;
    int blocks = (size + (threads * 2) - 1) / (threads * 2);
    size_t shared_memory = threads * sizeof(T);

    cudaMalloc(&d_vec, size * sizeof(T));
    cudaMalloc(&d_res, blocks * sizeof(T));

    cudaMemcpy(d_vec, vec, size * sizeof(T), cudaMemcpyHostToDevice);

    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);

    cudaEventRecord(start);
    int cur_size = size;
    T* cur_res = d_res;
    T* cur_vec = d_vec;
    while(1) {
        blocks = (cur_size + (threads * 2) - 1) / (threads * 2);

        vecSumKernel<<<blocks, threads, shared_memory>>>(cur_vec, cur_res, cur_size);
        cudaError_t err = cudaGetLastError();
        cudaDeviceSynchronize();
        
        if (blocks == 1) break;

        cur_size = blocks;
        T* tmp = cur_vec;
        cur_vec = cur_res;
        cur_res = tmp;
    }

    cudaMemcpy(res, cur_res, sizeof(T), cudaMemcpyDeviceToHost);
    cudaEventRecord(end);

    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);

    cudaFree(d_vec);
    cudaFree(d_res);

    cudaEventDestroy(start);
    cudaEventDestroy(end);
    
    return milliseconds;
}

template float launchVecSumKernel<float>(const float* vec, float* res, int size);
template float launchVecSumKernel<double>(const double* vec, double* res, int size);
template float launchVecSumKernel<int>(const int* vec, int* res, int size);
template float launchVecSumKernel<long>(const long* vec, long* res, int size);
template float launchVecSumKernel<long long>(const long long* vec, long long* res, int size);
