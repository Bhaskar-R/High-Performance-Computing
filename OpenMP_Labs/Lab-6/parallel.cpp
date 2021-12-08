#include <stdio.h>
#include <omp.h>

#define n 50000
#define delay 50000

int main()
{
	double a[n], b[n], c[n], runtime[11];
	float startTime, endTime;
	int i, k, omp_rank;
	double dot;
	int threads[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64};
	for (k = 0; k < 11; k++)
	{
		dot = 0.0;
		omp_set_num_threads(threads[k]);
		startTime = omp_get_wtime();
#pragma omp parallel private(i)
		{
#pragma omp for reduction(+ \
						  : dot)
			for (i = 0; i < n; i++)
			{
				omp_rank = omp_get_thread_num();
				a[i] = (float)i * 4.92;
				b[i] = (float)i * 2.37;
				c[i] = 0.0;
				for (int j = 0; j < delay; j++)
					c[i] += a[i] * b[i];
				dot += c[i];
			}
		}
		endTime = omp_get_wtime();
		runtime[k] = endTime - startTime;
	}
	for (k = 0; k < 11; k++)
		printf("\n\nThread Count: %d      Run Time: %f", threads[k], runtime[k]);
	return 0;
}
