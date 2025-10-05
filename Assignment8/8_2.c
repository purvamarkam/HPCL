#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000   // vector size

int main(int argc, char* argv[]) {
    int rank, size;
    int i;

    double *A, *B;
    double local_dot = 0.0, global_dot = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = N / size;

    // Allocate local arrays
    double *subA = (double*) malloc(chunk * sizeof(double));
    double *subB = (double*) malloc(chunk * sizeof(double));

    // Master initializes vectors
    if (rank == 0) {
        A = (double*) malloc(N * sizeof(double));
        B = (double*) malloc(N * sizeof(double));

        for (i = 0; i < N; i++) {
            A[i] = 1.0;   // simple values
            B[i] = 2.0;
        }
    }

    // Scatter data among processes
    MPI_Scatter(A, chunk, MPI_DOUBLE, subA, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunk, MPI_DOUBLE, subB, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ---- Start Timer ----
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // Each process computes local dot product
    for (i = 0; i < chunk; i++) {
        local_dot += subA[i] * subB[i];
    }

    // Reduce results into global dot product
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // ---- End Timer ----
    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Dot Product = %f\n", global_dot);
        printf("Execution Time with %d processes: %f seconds\n", size, end - start);
    }

    free(subA);
    free(subB);
    if (rank == 0) {
        free(A);
        free(B);
    }

    MPI_Finalize();
    return 0;
}