#include <iostream>
#include <ctime>
#include <omp.h>
using namespace std;
#define n 200
int main()
{
	int blocksize[] = {10, 25, 50, 75, 100, 125, 150};
	double a[n][n], b[n][n], c[n][n];
	float startTime, endTime, execTime;
	int B;
	for (int blocks = 0; blocks < 7; blocks++)
	{
		B = blocksize[blocks];
		printf("\nB = %d ", B);
		startTime = omp_get_wtime();
#pragma omp for collapse(2)
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i][j] = (i + 1) * 10.236;
				b[i][j] = (i + 1) * 152.123;
				c[i][j] = 0.0;
			}
		}
		for (int jj = 0; jj < n; jj = jj + B)
		{
			for (int kk = 0; kk < n; kk += B)
			{
				for (int i = 0; i < n; i++)
				{
					for (int j = jj; j < min(jj + B, n); j++)
					{
						double r = 0;
						for (int k = kk; k < min(kk + B, n); k++)
							r = r + a[i][k] * b[k][j];
						double temp = 3.0;
						for (int tt = 0; tt < n * 2; tt++)
							temp = tt * 3.2618 * 7.32347 / r;
						c[i][j] = c[i][j] + r;
					}
				}
			}
		}
		endTime = omp_get_wtime();
		execTime = endTime - startTime;
		printf("rtime = %f\n", execTime);
	}
	return 0;
}
