% % cu
#include <bits/stdc++.h>
	using namespace std;
#define N 1500
#define M 1024

__global__ void N_sum(float *a, float *b)
{
	__shared__ float temp[M];
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	if (index < N)
	{
		temp[threadIdx.x] = a[index];
		__syncthreads();
		if (threadIdx.x == 0)
		{
			float sum = 0;
			for (int i = 0; i < M; i++)
				sum += temp[i];
			atomicAdd(b, sum);
		}
	}
}

int main()
{
	srand(time(0));
	int blocks[] = {1, 1, 1, 1, 1, 1, 1, 10, 20, 30, 40, 50, M / 8, M / 4, M / 2, M, M, M, M, M};
	int threads[] = {1, 10, 20, 30, 40, 50, M, 10, 10, 10, 10, 10, M, M, M, M / 8, M / 4, M / 2, M};
	float a[N], b[N] = {0};
	float *d_a, *d_b;
	for (int i = 0; i < N; i++)
		a[i] = i + 1.2564;
	cudaMalloc((void **)&d_a, N * sizeof(float));
	cudaMalloc((void **)&d_b, N * sizeof(float));

	cudaMemcpy(d_a, a, N * sizeof(float), cudaMemcpyHostToDevice);

	for (int k = 0; k < 19; k++)
	{
		float elapsed = 0;
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);
		cudaEventRecord(start, 0);
		N_sum<<<blocks[k], threads[k]>>>(d_a, d_b);

		cudaError err = cudaMemcpy(b, d_b, N * sizeof(float), cudaMemcpyDeviceToHost);
		if (err != cudaSuccess)
			cout << "CUDA Error copying to Host: " << cudaGetErrorString(err);
		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&elapsed, start, stop);

		cudaEventDestroy(start);
		cudaEventDestroy(stop);
		printf("Blocks = %4d and Threads per Block = %4d Time = %.5f\n", blocks[k], threads[k], elapsed);
	}

	printf("\nNumbers : ");
	for (int i = 0; i < N; i++)
		cout << a[i] << "  ";

	printf("\nSum     : ");
	cout << b[0] / 19.0 << endl;
	cudaFree(d_a);
	cudaFree(d_b);

	return 0;
}