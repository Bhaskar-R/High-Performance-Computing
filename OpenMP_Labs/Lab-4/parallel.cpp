#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#define n 100
#define m 1000
int main()
{
	double a[n][n], b[n][n], c[n][n];
	float startTime, endTime, execTime;
	int i, k;
	int omp_rank;
	float rtime[20];
	int thread[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
	int thread_arr_size = 13;

#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = rand() % 500;
			b[i][j] = rand() % 500;
		}
	}

	for (k = 0; k < thread_arr_size; k++)
	{
		omp_set_num_threads(thread[k]);
		startTime = omp_get_wtime();
#pragma omp parallel private(i) shared(a, b, c)
		{
#pragma omp for
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					omp_rank = omp_get_thread_num();
					c[i][k] = 0;
					for (int l = 0; l < m; l++)
					{
						for (int k = 0; k < n; k++)
						{
							c[i][j] += a[i][k] * b[k][i];
						}
					}
				}
			}
		}
		endTime = omp_get_wtime();
		execTime = endTime - startTime;
		rtime[k] = execTime;
	}
	for (k = 0; k < thread_arr_size; k++)
		printf("\nThread=%d\t rtime=%f\n", thread[k], rtime[k]);
	return (0);
}
