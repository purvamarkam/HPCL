#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4   // Matrix size (change to bigger like 256, 512, etc.)

int main(int argc, char* argv[]) {
    int rank, size;
    int i, j, k;
    double A[N][N], B[N][N], C[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;
    int extra = N % size;

    // Master initializes A and B
    if (rank == 0) {
        printf("Matrix A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = i + j;   // simple values
                printf("%6.2f ", A[i][j]);
            }
            printf("\n");
        }

        printf("Matrix B:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                B[i][j] = i * j;   // simple values
                printf("%6.2f ", B[i][j]);
            }
            printf("\n");
        }
    }

    // Broadcast B to all processes
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A
    int sendcounts[size], displs[size];
    int sum = 0;
    for (i = 0; i < size; i++) {
        sendcounts[i] = (i < extra) ? (rows_per_process + 1) * N : rows_per_process * N;
        displs[i] = sum;
        sum += sendcounts[i];
    }

    int recvcount = sendcounts[rank];
    double *subA = (double *)malloc(recvcount * sizeof(double));
    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE, subA, recvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int local_rows = recvcount / N;
    double *subC = (double *)malloc(local_rows * N * sizeof(double));

    // ---- Start timer ----
    MPI_Barrier(MPI_COMM_WORLD);  // synchronize before timing
    double start = MPI_Wtime();

    // Local computation
    for (i = 0; i < local_rows; i++) {
        for (j = 0; j < N; j++) {
            subC[i*N + j] = 0;
            for (k = 0; k < N; k++) {
                subC[i*N + j] += subA[i*N + k] * B[k][j];
            }
        }
    }

    // Gather results back to master
    MPI_Gatherv(subC, local_rows * N, MPI_DOUBLE, C, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ---- End timer ----
    MPI_Barrier(MPI_COMM_WORLD);  // synchronize after computation
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Result C = A*B:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%6.2f ", C[i][j]);
            }
            printf("\n");
        }
        printf("Execution Time with %d processes: %f seconds\n", size, end - start);
    }

    free(subA);
    free(subC);
    MPI_Finalize();
    return 0;
}