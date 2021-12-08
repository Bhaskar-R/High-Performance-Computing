% % cu
#include <bits/stdc++.h>
	using namespace std;
#define N 25
#define M 1024

__global__ void vector_mul(double *a, double *b, double *c)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if (id < N)
		c[id] = a[id] * b[id];
}

int main()
{
	srand(time(0));
	int blocks[] = {1, 1, 1, 1, 1, 1, 1, 10, 20, 30, 40, 50, M / 2, M / 4, M / 8, M, M, M, M, M};
	int threads[] = {1, 10, 20, 30, 40, 50, M, 10, 10, 10, 10, 10, M, M, M, M / 2, M / 4, M / 8, M};
	double a[N], b[N], c[N];
	double *d_a, *d_b, *d_c;
	double size = N * sizeof(double);

	// Allocate space for device copies of a, b, c
	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMalloc((void **)&d_c, size);

	for (int i = 0; i < N; i++)
	{
		a[i] = rand() % 100 + i + 0.250;
		b[i] = rand() % 100 * i + 0.248;
	}

	cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);

	for (int k = 0; k < 19; k++)
	{
		float elapsed = 0;
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

		cudaEventRecord(start, 0);
		vector_mul<<<blocks[k], threads[k]>>>(d_a, d_b, d_c);

		// Copy result back to host
		cudaError err = cudaMemcpy(&c, d_c, size, cudaMemcpyDeviceToHost);
		if (err != cudaSuccess)
			cout << "CUDA Error copying to Host :" << cudaGetErrorString(err) << endl;

		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&elapsed, start, stop);

		cudaEventDestroy(start);
		cudaEventDestroy(stop);

		printf("Blocks = %4d and Threads per Block = %4d Time = %.5f\n", blocks[k], threads[k], elapsed);
	}
	cout << "\nProduct of Vectors " << endl;
	for (int i = 0; i < N; i++)
		cout << a[i] << " * " << b[i] << " = " << c[i] << endl;

	// Cleanup
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;
}