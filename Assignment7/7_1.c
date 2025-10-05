#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double rand_double() {
    return (double)rand() / RAND_MAX;
}

int main(int argc, char** argv) {
    int rank, size;
    int N = 4; // default size

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1) {
        N = atoi(argv[1]); // allow size from command line
    }

    double *A = NULL, *x = NULL, *y = NULL;
    double *row = (double*)malloc(N * sizeof(double));
    double *vec = (double*)malloc(N * sizeof(double));
    double result;

    if (rank == 0) {
        A = (double*)malloc(N * N * sizeof(double));
        x = (double*)malloc(N * sizeof(double));
        y = (double*)malloc(N * sizeof(double));

        srand(time(NULL));
        for (int i = 0; i < N * N; i++) A[i] = rand_double();
        for (int i = 0; i < N; i++) x[i] = rand_double();
    }

    MPI_Barrier(MPI_COMM_WORLD); // sync before timing
    double start = MPI_Wtime();

    if (rank == 0) {
        // Send vector to all workers
        for (int p = 1; p < N && p < size; p++) {
            MPI_Send(x, N, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        // Keep copy
        for (int j = 0; j < N; j++) vec[j] = x[j];

        // Send rows to each worker
        for (int p = 1; p < N && p < size; p++) {
            MPI_Send(&A[p*N], N, MPI_DOUBLE, p, 1, MPI_COMM_WORLD);
        }

        // Compute row 0 locally
        result = 0.0;
        for (int j = 0; j < N; j++) result += A[0*N+j] * vec[j];
        y[0] = result;

        // Receive results
        for (int p = 1; p < N && p < size; p++) {
            double val;
            MPI_Recv(&val, 1, MPI_DOUBLE, p, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            y[p] = val;
        }
    } 
    else if (rank < N) {
        // Workers
        MPI_Recv(vec, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(row, N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        result = 0.0;
        for (int j = 0; j < N; j++) result += row[j] * vec[j];

        MPI_Send(&result, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD); // sync before stop
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("N=%d, processes=%d, time=%.6f sec\n", N, size, end - start);

        // Optional: print result vector for verification (only for small N)
        if (N <= 8) {
            printf("Result y = A * x:\n");
            for (int i = 0; i < N; i++) printf("%6.2f ", y[i]);
            printf("\n");
        }

        free(A); free(x); free(y);
    }

    free(row); free(vec);
    MPI_Finalize();
    return 0;
}
