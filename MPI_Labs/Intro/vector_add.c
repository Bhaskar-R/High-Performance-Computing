#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[])
{
	int tasks, taskId, numWorkers;
	int source, dest, mtype, rows;
	int step, extra, offset, i, j, k, rc;
	long double a[SIZE], b[SIZE], c[SIZE];
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
	MPI_Comm_size(MPI_COMM_WORLD, &tasks);
	if(tasks < 2)
	{
		printf("Need at least 2 MPI tasks. Quit..");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	numWorkers = tasks -1;
	
	// Master task
	if(taskId == MASTER)
	{
		for(i=0; i<SIZE; i++)
		a[i] = i;
		for(i=0; i<SIZE; i++)
		b[i] = i;
		
		// Send data to workers
		step = SIZE/numWorkers;
		extra = SIZE%numWorkers;
		offset = 0;
		mtype = FROM_MASTER;
		for(dest=1; dest<=numWorkers; dest++)
		{
			rows = (dest <= extra) ? step+1 : step;
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset], rows, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&b[offset], rows, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset += rows;
		}
		
		// Receive results from workers
		mtype = FROM_WORKER;
		for(i=1; i<=numWorkers; i++)
		{
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset], rows, MPI_LONG_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
		}
		
		//Printing results
		printf("*****************\n");
		printf("Vector Addition:\n");
		for(i=0; i<SIZE; i++)
		printf("%6.2Lf\t%6.2Lf\t%6.2Lf\n", a[i], b[i], c[i]);
		printf("*****************\n");
		printf("Done\n");
	}
	
	// Worker task
	if(taskId > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, rows, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		
		for(i=0; i<rows; i++)
		c[i] = a[i] + b[i];
		
		mtype = FROM_WORKER;
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&c, rows, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
