#include <mpi.h>
#include <stdio.h>

int main()
{
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(pro_name, &name_len);
	
	printf("Hello World from Processor %s, rank %d, out of %d processors\n", pro_name, rank, world_size);
	
	MPI_Finalize();
}
