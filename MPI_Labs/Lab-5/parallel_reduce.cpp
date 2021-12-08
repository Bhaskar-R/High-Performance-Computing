#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 100
//main function
int main(int argc, char **argv)
{
	int myid, numprocs;
	int i, x, start, end, rem;
	float val = 0, result, arr[N];
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
			arr[i] = i;
	}
	/* broadcast arr */
	MPI_Bcast(arr, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	/* add portion of arr */
	x = N / numprocs;
	start = myid * x;
	end = start + x;
	for (i = start; i < end; i++)
	{
		val += arr[i];
	}
	printf("Calculated %f in %d - %s\n", val, myid, pro_name);
	/* compute global sum */
	MPI_Reduce(&val, &result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (0 == myid)
	{
		rem = N % numprocs;
		for (i = N - rem; i < N; i++)
			result += arr[i];
		printf("Sum is %f.\n", result);
		ed = MPI_Wtime();
		printf("\nTime= %f", ed - st);
	}
	MPI_Finalize();
}