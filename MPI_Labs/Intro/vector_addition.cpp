#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int myid, numprocs;
	double startwtime, endwtime;
	int namelen;
	int ARRAY_SIZE = 10000;
	int vector1[ARRAY_SIZE + 50];
	int vector2[ARRAY_SIZE + 50];
	long int vector3[ARRAY_SIZE + 50];
	int i, j;
	int s, s0;
	double totalTime;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);
	fprintf(stderr, "Process %d on %s\n", myid, processor_name);
	fflush(stderr);
	// Vector 1 Reading
	for (i = 0; i < ARRAY_SIZE; i++)
		vector1[i] = i;
	// Vector 2 Reading
	for (i = 0; i < ARRAY_SIZE; i++)
		vector2[i] = i;

	if (myid == 0)
	{
		MPI_Bcast(&ARRAY_SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD);
		s = (int)floor(ARRAY_SIZE / numprocs);
