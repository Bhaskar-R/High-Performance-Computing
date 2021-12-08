#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 100
//parallel /cluster code
int main(int argc, char **argv)
{
	int myid, numprocs;
	int i, x, start, end, rem;
	long double val = 0, result, arr1[N], arr2[N];
	double st, ed;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(pro_name, &name_len);
	if (0 == myid)
	{
		st = MPI_Wtime();
		for (i = 0; i < N; i++)
		{
			arr1[i] = i;
			arr2[i] = i;
		}
	}
	/* broadcast arr */
	MPI_Bcast(arr1, N, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(arr2, N, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
	/* add portion of arr */
	x = N / numprocs;
	start = myid * x;
	end = start + x;
	for (i = start; i < end; i++)
	{
		val += (arr1[i] * arr2[i]);
	}
	printf("Calculated %Lf in %d - %s\n", val, myid, pro_name);
	/* compute global sum */
	MPI_Reduce(&val, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (0 == myid)
	{
		rem = N % numprocs;
		for (i = N - rem; i < N; i++)
			result += (arr1[i] * arr2[i]);
		printf("Dot product is %Lf.\n", result);
		ed = MPI_Wtime();
		printf("\nTime= %f", ed - st);
	}
	MPI_Finalize();
}