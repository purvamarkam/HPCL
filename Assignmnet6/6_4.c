#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size, send_data, recv_data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process sends its rank to its right neighbor
    send_data = rank;
    int right = (rank + 1) % size;
    int left = (rank - 1 + size) % size;

    // Send to right, receive from left (blocking)
    MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_data, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received %d from process %d\n", rank, recv_data, left);

    MPI_Finalize();
    return 0;
}