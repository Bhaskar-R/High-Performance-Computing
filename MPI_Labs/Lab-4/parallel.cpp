#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2
#define MatA_rows 5
#define MatA_cols 5
#define MatB_cols 5
// cluster /parallel code
int main(int argc, char *argv[])
{
	int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j,
		k, rc;
	long double a[MatA_rows][MatA_cols], b[MatA_cols][MatB_cols],
		c[MatA_rows][MatB_cols];
	MPI_Status status;
	double start, end;
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	if (numtasks < 2)
	{
		printf("Need at least two MPI tasks\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	numworkers = numtasks - 1;
	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(pro_name, &name_len);
	printf("Working in Processor %s\n", pro_name);
	if (taskid == MASTER)
	{
		for (i = 0; i < MatA_rows; i++)
			for (j = 0; j < MatA_cols; j++)
				a[i][j] = i + j;
		for (i = 0; i < MatA_cols; i++)
			for (j = 0; j < MatB_cols; j++)
			{
				b[i][j] = i * j;
				c[i][j] = 0.0;
			}
		averow = MatA_rows / numworkers;
		extra = MatA_rows % numworkers;
		offset = 0;
		mtype = FROM_MASTER;
		for (dest = 1; dest <= numworkers; dest++)
		{
			rows = (dest <= extra) ? averow + 1 : averow;
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows * MatA_cols, MPI_LONG_DOUBLE, dest, mtype,
					 MPI_COMM_WORLD);
			MPI_Send(&b, MatA_cols * MatB_cols, MPI_LONG_DOUBLE, dest, mtype,
					 MPI_COMM_WORLD);
			offset = offset + rows;
		}
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++)
		{
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows * MatB_cols, MPI_LONG_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
		}
		printf("Result Matrix:\n");
		for (i = 0; i < MatA_rows; i++)
		{
			printf("\n");
			for (j = 0; j < MatB_cols; j++)
				printf("%3.2Lf ", c[i][j]);
		}
		end = MPI_Wtime();
		printf("\nTime= %f", end - start);
	}
	if (taskid > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows * MatA_cols, MPI_LONG_DOUBLE, MASTER, mtype,
				 MPI_COMM_WORLD, &status);
		MPI_Recv(&b, MatA_cols * MatB_cols, MPI_LONG_DOUBLE, MASTER, mtype,
				 MPI_COMM_WORLD, &status);
		for (k = 0; k < MatB_cols; k++)
			for (i = 0; i < rows; i++)
			{
				c[i][k] = 0.0;
				for (j = 0; j < MatA_cols; j++)
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
			}
		mtype = FROM_WORKER;
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&c, rows * MatB_cols, MPI_LONG_DOUBLE, MASTER, mtype,
				 MPI_COMM_WORLD);
	}
	MPI_Finalize();
}