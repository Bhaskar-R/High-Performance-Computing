#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define n 100
#define m 100
int main()
{
	double a[n], b[n], c[n];
	float startTime, endTime, execTime;
	int i, k;
	float rtime;
	startTime = MPI_WTIME();
	for (i = 0; i < n; i++)
	{
		a[i] = i * 10.236;	// Use Random function and assign a[i]
		b[i] = i * 152.123; // Use Random function and assign b[i]
		for (int j = 0; j < m; j++)
			c[i] = a[i] * b[i];
		//printf("The value of a[%d] = %lf and b[%d] = %lf and result c[%d] = %lf done by worker Thread ID = % d\n ", i, a[i], i, b[i], i, c[i], omp_rank);
	}
	endTime = MPI_WTIME();
	execTime = endTime - startTime;
	rtime = execTime;
	printf("\n rtime=%f\n", rtime);
	return (0);
}