#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int me, nproc, ierr;
	MPI_Init(&ierr, &argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	for(int i=0; i<nproc; i++) {
		if(me==i) {
			printf("Thread %d Hello World!\n", me);
		}
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)
	return 0;
}