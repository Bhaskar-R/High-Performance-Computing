#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 100
int main()
{
	double start, end;
	start = MPI_Wtime();
	float arr1[N], arr2[N], result = 0.0;
	for (int i = 0; i < N; i++)
	{
		arr1[i] = i + 1.0;
		arr2[i] = i + 1.0;
	}
	for (int i = 0; i < N; i++)
	{
		result += (arr1[i] * arr2[i]);
	}
	printf("Sum is %f.\n", result);
	end = MPI_Wtime();
	printf("\nTime= %f", end - start);
}