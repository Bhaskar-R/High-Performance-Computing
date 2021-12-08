#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#define N 100000
#define DELAY 100000
int threads[] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64};
double rtime[11];
int main()
{
	double fsum, psum, a[N];
	double start, end;
	int i, k, l;
	for (k = 0; k < 11; k++)
	{
		omp_set_num_threads(threads[k]);
		fsum = 0.0;
		start = omp_get_wtime();
#pragma omp parallel
		{
			double psum = 0.0;
#pragma omp for
			for (i = 0; i < N; i++)
			{
				a[i] = (float)(i % 50) * 1.76;
				for (int kk = 0; kk < DELAY; kk++)
					;
				psum = psum + a[i];
			}
#pragma omp critical
			fsum = fsum + psum;
		}
		end = omp_get_wtime();
		rtime[k] = end - start;
		//printf("FSUM = ",fsum);
	}
	for (l = 0; l < 11; l++)
		printf("\nThread=%d\t rtime=%f\n", threads[l], rtime[l]);
	return 0;
}
