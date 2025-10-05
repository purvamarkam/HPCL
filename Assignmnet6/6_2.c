#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 5) {
        printf("Process %d belongs to communicator group: MPI_COMM_WORLD\n", rank);
    } else {
        if (rank == 0) {
            printf("Please run the program with 5 processes.\n");
        }
    }

    MPI_Finalize();
    return 0;
}