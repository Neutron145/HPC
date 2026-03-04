#include <iostream>

template <typename T>
__global__ void matMulKernel(const T* A, const T* B, T* C, int M, int K, int N) {
    //!< Отображаем координаты потока внутри блока и координаты блока в координаты элемента матрицы
    int row = blockDim.y * blockIdx.y + threadIdx.y;
    int col = blockDim.x * blockIdx.x + threadIdx.x;

    if (row < M && col < N) {
        T sum = 0;
        for (int k = 0; k < K; k++) {
            sum += A[row * K + k] * B[k * N + col];
        }
        C[row * M + col] = sum;
    }
}

template <typename T>
float launchMatMulKernel(const T* A, const T* B, T* C, int M, int K, int N) {
    T *d_A, *d_B, *d_C;
    int sizeA = M * K * sizeof(T);
    int sizeB = K * N * sizeof(T);
    int sizeC = M * N * sizeof(T);

    cudaMalloc(&d_A, sizeA);
    cudaMalloc(&d_B, sizeB);
    cudaMalloc(&d_C, sizeC);
    
    cudaMemcpy(d_A, A, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeB, cudaMemcpyHostToDevice);

    dim3 threads(16, 16); 
    
    dim3 blocks((N + threads.x - 1) / threads.x, (M + threads.y - 1) / threads.y);

    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);

    cudaEventRecord(start);
    matMulKernel<<<blocks, threads>>>(d_A, d_B, d_C, M, K, N);
    cudaEventRecord(end);

    cudaMemcpy(C, d_C, sizeC, cudaMemcpyDeviceToHost);

    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    cudaEventDestroy(start);
    cudaEventDestroy(end);
    
    return milliseconds;
}

template float launchMatMulKernel<float>(const float*, const float*, float*, int, int, int);
template float launchMatMulKernel<double>(const double*, const double*, double*, int, int, int);
template float launchMatMulKernel<int>(const int*, const int*, int*, int, int, int);
template float launchMatMulKernel<long>(const long*, const long*, long*, int, int, int);
template float launchMatMulKernel<long long>(const long long*, const long long*, long long*, int, int, int);
