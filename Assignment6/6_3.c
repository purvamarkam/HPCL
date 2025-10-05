#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int msg = 0;

    if (rank == 0) {
        // Process 0 tries to send first, then receive
        MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        // Process 1 also tries to send first, then receive
        MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}