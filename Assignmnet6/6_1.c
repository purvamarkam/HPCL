#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);//Sets up everything needed for MPI communication between processes.
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //rank of curr MIP process
    MPI_Comm_size(MPI_COMM_WORLD, &size); //total number of MPI processes

    printf("Hello from process %d of %d\n", rank, size);

    MPI_Finalize();//Cleans up the MPI environment.
    return 0;
}