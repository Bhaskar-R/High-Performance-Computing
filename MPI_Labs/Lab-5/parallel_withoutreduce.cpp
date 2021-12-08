#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 100
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2
int main(int argc, char *argv[])
{
	int numtasks, taskid, numworkers, source, dest, mtype,
		segment, aveseg, extra, offset, i, j, k, rc;
	long double a[SIZE], b[SIZE], sum = 0.0, temp = 0.0; //b is temporary
	MPI_Status status;
	double start, end;
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	if (numtasks < 2)
	{
		printf("Need atleast two MPI tasks. Quiting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(pro_name, &name_len);
	printf("-From %s, rank %d, out of %d processors\n", pro_name, taskid, numtasks);
	numworkers = numtasks - 1;
	//master task:
	if (taskid == MASTER)
	{
		for (i = 0; i < SIZE; i++)
		{
			a[i] = i;
		}
		aveseg = SIZE / numworkers;
		extra = SIZE % numworkers;
		offset = 0;
		mtype = FROM_MASTER;
		for (dest = 1; dest <= numworkers; dest++)
		{
			segment = (dest <= extra) ? aveseg + 1 : aveseg;
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&segment, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset], segment, MPI_LONG_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset = offset + segment;
		}
		//receive from worker:
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++)
		{
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&segment, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&temp, 1, MPI_LONG_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
			sum += temp;
		}
		printf("Sum is %Lf.\n", sum);
		end = MPI_Wtime();
		printf("\nTime= %f\n", end - start);
	}
	//Worker task:
	if (taskid > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&segment, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, segment, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		temp = 0.0;
		for (i = 0; i < segment; i++)
			temp += a[i];
		mtype = FROM_WORKER;
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&segment, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&temp, 1, MPI_LONG_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}