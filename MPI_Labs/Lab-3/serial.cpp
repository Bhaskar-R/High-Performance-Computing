#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define NR 5
#define NC 5
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2
int main()
{
	double start, end;
	int i, j;
	long double a[NR][NC], b[NC][NC], c[NR][NC];
	start = MPI_Wtime();
	for (i = 0; i < NR; i++)
		for (j = 0; j < NC; j++)
			a[i][j] = i + j * 1.785;
	for (i = 0; i < NR; i++)
		for (j = 0; j < NC; j++)
			b[i][j] = i + j * 0.987;
	for (i = 0; i < NR; i++)
		for (j = 0; j < NC; j++)
		{
			c[i][j] = a[i][j] + b[i][j];
		}
	printf("\nResultant Matrix:\n");
	for (i = 0; i < NR; i++)
	{
		printf("\n");
		for (j = 0; j < NC; j++)
			printf("%3.1Lf ", c[i][j]);
	}
	printf("\nFinished.\n");
	end = MPI_Wtime();
	printf("\nTime= %f", end - start);
	return 0;
}