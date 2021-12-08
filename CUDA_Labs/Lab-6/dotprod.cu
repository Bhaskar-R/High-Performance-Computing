% % cu
#include <bits/stdc++.h>
	using namespace std;
#define N 1500
#define M 1024

__global__ void dot_product(float *a, float *b, float *c)
{
	__shared__ float temp[M];

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	if (index < N)
	{
		temp[threadIdx.x] = a[index] * b[index];
		__syncthreads();
		if (threadIdx.x == 0)
		{
			float sum = 0.0;
			if (blockIdx.x < N / blockDim.x)
			{
				for (int i = 0; i < (int)blockDim.x; i++)
					sum += temp[i];
			}
			else
			{
				for (int i = 0; i < N % blockDim.x; i++)
					sum += temp[i];
			}
			atomicAdd(c, sum);
		}
	}
}

int main()
{
	srand(time(0));
	int blocks[] = {1, 1, 1, 1, 1, 1, 1, 10, 20, 30, 40, 50, M / 8, M / 4, M / 2, M, M, M, M, M};
	int threads[] = {1, 10, 20, 30, 40, 50, M, 10, 10, 10, 10, 10, M, M, M, M / 8, M / 4, M / 2, M};
	float a[N], b[N], c[N];
	float *d_a, *d_b, *d_c;
	for (int i = 0; i < N; i++)
	{
		a[i] = i + 0.250;
		b[i] = i + 0.248;
	}

	cudaMalloc((void **)&d_a, N * sizeof(float));
	cudaMalloc((void **)&d_b, N * sizeof(float));
	cudaMalloc((void **)&d_c, N * sizeof(float));

	cudaMemcpy(d_a, a, N * sizeof(float), cudaMemcpyHostToDevice);

	for (int k = 0; k < 19; k++)
	{
		float elapsed = 0;
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);
		cudaEventRecord(start, 0);
		dot_product<<<blocks[k], threads[k]>>>(d_a, d_b, d_c);

		cudaError err = cudaMemcpy(c, d_c, N * sizeof(float), cudaMemcpyDeviceToHost);
		if (err != cudaSuccess)
			cout << "CUDA Error copying to Host: " << cudaGetErrorString(err);
		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&elapsed, start, stop);

		cudaEventDestroy(start);
		cudaEventDestroy(stop);
		printf("Blocks = %4d and Threads per Block = %4d Time = %.5f\n", blocks[k], threads[k], elapsed);
	}
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	return 0;
}