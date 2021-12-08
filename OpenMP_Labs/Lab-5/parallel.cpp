#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#define n 10000
int main()
{
	double a[n], rtime[n], sumarr[n];
	float startTime, endTime, execTime;
	int i, k, omp_rank;
	double sum;
	int thread[] = {1, 2, 4, 6, 8, 12, 16, 20, 32, 64};
	int thread_arr_size = 10;
	for (k = 0; k < thread_arr_size; k++)
	{
		sum = 0.0;
		omp_set_num_threads(thread[k]);
		startTime = omp_get_wtime();
#pragma omp parallel private(i)
		{
#pragma omp for reduction(+ \
						  : sum)
			for (i = 0; i < n; i++)
			{
				omp_rank = omp_get_thread_num();
				a[i] = (float)i * 1.67;
				for (int j = 0; j < 100010; j++)
					a[i] = a[i] + 2;
				sum = sum + a[i];
			}
		}
		endTime = omp_get_wtime();
		execTime = endTime - startTime;
		rtime[k] = execTime;
		sumarr[k] = sum;
	}
	for (k = 0; k < thread_arr_size; k++)
		printf("\nThread = %d\t rtime = %f\n", thread[k], rtime[k]);
	return 0;
}