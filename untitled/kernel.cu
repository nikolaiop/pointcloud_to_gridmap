#include <cuda_runtime.h>
#include <iostream>
#include <chrono>

void initialize_matrix_cpu(float* matrix, int rows, int cols);
__global__ void initialize_matrix_gpu(float* matrix, int rows, int cols);

int main() {
    const int rows = 100000;
    const int cols = 100000;
    float* matrix_cpu = new float[rows * cols];
    float* matrix_gpu;

    // Initialize the matrix on the CPU
    auto start_cpu = std::chrono::high_resolution_clock::now();
    initialize_matrix_cpu(matrix_cpu, rows, cols);
    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_cpu = end_cpu - start_cpu;
    std::cout << "CPU execution time: " << elapsed_cpu.count() << " ms\n";

    // Allocate memory on the GPU
    cudaMalloc((void**)&matrix_gpu, rows * cols * sizeof(float));

    // Initialize the matrix on the GPU
    dim3 block_dim(32, 32);
    dim3 grid_dim((cols + block_dim.x - 1) / block_dim.x, (rows + block_dim.y - 1) / block_dim.y);
    auto start_gpu = std::chrono::high_resolution_clock::now();
    initialize_matrix_gpu<<<grid_dim, block_dim>>>(matrix_gpu, rows, cols);
    cudaDeviceSynchronize();
    auto end_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_gpu = end_gpu - start_gpu;
    std::cout << "GPU execution time: " << elapsed_gpu.count() << " ms\n";

    // Free the memory on the GPU
    cudaFree(matrix_gpu);

    // Free the memory on the CPU
    delete[] matrix_cpu;

    return 0;
}

void initialize_matrix_cpu(float* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = 1.0f; // Initialize the value to 1
    }
}

__global__ void initialize_matrix_gpu(float* matrix, int rows, int cols) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows && col < cols) {
        int index = row * cols + col;
        matrix[index] = 1.0f; // Initialize the value to 1
    }
}