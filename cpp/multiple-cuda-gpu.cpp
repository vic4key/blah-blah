#include <iostream>
#include <vector>
#include <thread>
#include <cuda_runtime.h>

// A function to check CUDA errors
void checkCudaError(cudaError_t err, const char *msg)
{
  if (err != cudaSuccess)
  {
    std::cerr << "CUDA error: " << msg << ": " << cudaGetErrorString(err) << "\n";
    exit(EXIT_FAILURE);
  }
}

// A kernel function to perform vector addition on the GPU
global void vectorAdd(const float *a, const float *b, float *c, int n)
{
  int i = blockIdx.x *blockDim.x + threadIdx.x;
  if (i < n)
  {
    c[i] = a[i] + b[i];
  }
}

// A function to perform vector addition on a subset of the data using a specific GPU
void vectorAddOnGPU(const float *a, const float *b, float *c, int n, int gpu)
{
  // Set the device to use
  checkCudaError(cudaSetDevice(gpu), "cudaSetDevice");

  // Allocate device memory
  float *d_a = nullptr;
  float *d_b = nullptr;
  float *d_c = nullptr;
  checkCudaError(cudaMalloc(&d_a, n* sizeof(float)), "cudaMalloc");
  checkCudaError(cudaMalloc(&d_b, n* sizeof(float)), "cudaMalloc");
  checkCudaError(cudaMalloc(&d_c, n* sizeof(float)), "cudaMalloc");

  // Copy data from host to device
  checkCudaError(cudaMemcpy(d_a, a, n* sizeof(float), cudaMemcpyHostToDevice), "cudaMemcpy");
  checkCudaError(cudaMemcpy(d_b, b, n* sizeof(float), cudaMemcpyHostToDevice), "cudaMemcpy");

  // Launch the kernel
  int blockSize = 256;
  int numBlocks = (n + blockSize - 1) / blockSize;
  vectorAdd <<<numBlocks, blockSize>>> (d_a, d_b, d_c, n);
  checkCudaError(cudaGetLastError(), "vectorAdd");

  // Copy data from device to host
  checkCudaError(cudaMemcpy(c, d_c, n* sizeof(float), cudaMemcpyDeviceToHost), "cudaMemcpy");

  // Free device memory
  checkCudaError(cudaFree(d_a), "cudaFree");
  checkCudaError(cudaFree(d_b), "cudaFree");
  checkCudaError(cudaFree(d_c), "cudaFree");
}

// A function to print a vector
void printVector(const std::vector<float> &v)
{
  for (float x: v)
  {
    std::cout << x << " ";
  }

  std::cout << "\n";
}

int main()
{
  // The size of the input data
  int N = 1024;

  // The number of GPUs to use
  int numGPUs = 4;

  // The size of the data for each GPU
  int sizePerGPU = N / numGPUs;

  // Create some input vectors on the host
  std::vector<float> h_a(N);
  std::vector<float> h_b(N);
  std::vector<float> h_c(N);

  // Initialize the input vectors
  for (int i = 0; i < N; i++)
  {
    h_a[i] = i;
    h_b[i] = i * 2;
  }

  // Print the input vectors
  std::cout << "Input vectors:\n";
  printVector(h_a);
  printVector(h_b);

  // Create a vector of threads
  std::vector<std::thread > threads;

  // Launch a thread for each GPU
  for (int i = 0; i < numGPUs; i++)
  {
    // Get the pointers to the data for this GPU
    const float *a = h_a.data() + i * sizePerGPU;
    const float *b = h_b.data() + i * sizePerGPU;
          float *c = h_c.data() + i * sizePerGPU;

    // Create and start the thread
    threads.emplace_back(vectorAddOnGPU, a, b, c, sizePerGPU, i);
  }

  // Wait for all threads to finish
  for (auto &t: threads)
  {
    t.join();
  }

  // Synchronize all GPUs
  checkCudaError(cudaDeviceSynchronize(), "cudaDeviceSynchronize");

  // Print the output vector
  std::cout << "Output vector:\n";
  printVector(h_c);

  return 0;
}