#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define n 100
#define m 100000
int main()
{
	double a[n][n], b[n][n], c[n][n];
	float startTime, endTime, execTime;
	int i, k;
	int omp_rank;
	float rtime;
	startTime = omp_get_wtime();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = rand() % 500;
			b[i][j] = rand() % 500;
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			for (int k = 0; k < m; k++)
				c[i][j] = a[i][j] + b[i][j];
	}
	endTime = omp_get_wtime();
	execTime = endTime - startTime;
	rtime = execTime;
	printf("\n rtime=%f\n", rtime);
	return (0);
}
