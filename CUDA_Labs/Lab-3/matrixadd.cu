% % cu
#include <bits/stdc++.h>
	using namespace std;
#define N 7
#define M 1024

__global__ void matadd(double a[][N], double b[][N], double c[][N])
{
	int id = gridDim.x * blockIdx.y + blockIdx.x;
	if (id < N)
	{
		for (int i = 0; i < N; i++)
			c[id][i] = a[id][i] + b[id][i];
	}
}

int main()
{
	srand(time(0));
	int blocks[] = {1, 1, 1, 1, 1, 1, 1, 10, 20, 30, 40, 50, M / 8, M / 4, M / 2, M, M, M, M, M};
	int threads[] = {1, 10, 20, 30, 40, 50, M, 10, 10, 10, 10, 10, M, M, M, M / 8, M / 4, M / 2, M};
	double a[N][N], b[N][N], c[N][N] = {{0}};
	double(*d_a)[N], (*d_b)[N], (*d_c)[N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			a[i][j] = i + j + 0.250;
			b[i][j] = i + j + 0.248;
		}
	}
	cudaMalloc((void **)&d_a, N * N * sizeof(double));
	cudaMalloc((void **)&d_b, N * N * sizeof(double));
	cudaMalloc((void **)&d_c, N * N * sizeof(double));

	cudaMemcpy(d_a, a, N * N * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, N * N * sizeof(double), cudaMemcpyHostToDevice);
	dim3 grid(N, N);

	for (int k = 0; k < 19; k++)
	{
		float elapsed = 0;
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);
		cudaEventRecord(start, 0);
		matadd<<<blocks[k], threads[k]>>>(d_a, d_b, d_c);

		cudaError err = cudaMemcpy(c, d_c, N * N * sizeof(double), cudaMemcpyDeviceToHost);
		if (err != cudaSuccess)
			cout << "CUDA Error copying to Host: " << cudaGetErrorString(err);
		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&elapsed, start, stop);

		cudaEventDestroy(start);
		cudaEventDestroy(stop);
		printf("Blocks = %4d and Threads per Block = %4d Time = %.5f\n", blocks[k], threads[k], elapsed);
	}

	printf("\nMatrix A:\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)

			cout << a[i][j] << "\t";

		cout << endl;
	}
	printf("\nMatrix B:\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << b[i][j] << "\t";
		cout << endl;
	}
	printf("\nSum :\n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << c[i][j] << "\t";
		cout << endl;
	}
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	return 0;
}