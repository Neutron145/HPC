#include <image.h>


__global__ void bilateralKernel(cudaTextureObject_t texObj, uint8_t *out, int width, int height, float sigma_r, float sigma_d, int pad_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    if (idx >= width || idy >= height) return;
    
    float h = 0.0f;
    float k = 0.0f;
    float f_a0 = tex2D<uint8_t>(texObj, idx, idy);
    for (int i = -pad_size; i <= pad_size; i++) {
        for (int j = -pad_size; j <= pad_size; j++) {
            float g = expf(-(i * i + j * j) / (sigma_d * sigma_d));
            float f_ai = tex2D<uint8_t>(texObj, idx + i, idy + j);
            float r = expf(-((f_ai - f_a0) * (f_ai - f_a0)) / (sigma_r * sigma_r));
            k += g * r;
            h += g * r * f_ai;
        }
    }
    out[idy * width + idx] = static_cast<uint8_t>(h / k);
}


float launchBilateralKernel(const Image& input, Image& output, float sigma_r, float sigma_d, int pad_size) {
    output.width = input.width;
    output.height = input.height;
    output.num_channels = input.num_channels;
    output.data.resize(output.width * output.height * output.num_channels, 0);

    uint8_t *d_out;
    cudaMalloc(&d_out, output.width * output.height * sizeof(uint8_t));

    cudaArray_t d_input;
    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc(8, 0, 0, 0, cudaChannelFormatKindUnsigned);
    cudaMallocArray(&d_input, &channelDesc, input.width, input.height);

    cudaResourceDesc resDesc = {};
    resDesc.resType = cudaResourceTypeArray;
    resDesc.res.array.array = d_input;

    cudaTextureDesc texDesc = {};
    texDesc.addressMode[0] = cudaAddressModeClamp;
    texDesc.addressMode[1] = cudaAddressModeClamp;
    texDesc.filterMode = cudaFilterModePoint;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = 0;
    cudaTextureObject_t texObj;

    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);
    cudaEventRecord(start);

    dim3 blockDim(16, 16);
    dim3 gridDim((output.width + blockDim.x - 1) / blockDim.x,
                    (output.height + blockDim.y - 1) / blockDim.y);

    for (int i = 0; i < output.num_channels; i++) {
        cudaMemcpy2DToArray(d_input, 0, 0, input.data.data() + input.width * input.height * i,
                            input.width * sizeof(uint8_t),
                            input.width * sizeof(uint8_t),
                            input.height, cudaMemcpyHostToDevice);
        
        cudaCreateTextureObject(&texObj, &resDesc, &texDesc, nullptr);

        bilateralKernel<<<gridDim, blockDim>>>(texObj, d_out, output.width, output.height, sigma_r, sigma_d, pad_size);
        
        cudaMemcpy(output.data.data() + output.width * output.height * i, d_out, output.width * output.height * sizeof(uint8_t), cudaMemcpyDeviceToHost);
        cudaDestroyTextureObject(texObj);
    }

    cudaEventRecord(end);
    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);
    cudaEventDestroy(start);
    cudaEventDestroy(end);

    cudaFreeArray(d_input);
    cudaFree(d_out);

    return milliseconds;
}