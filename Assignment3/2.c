// Problem 2: Matrix Addition with malloc, collapse, schedule, and speedup analysis
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_addition(int size) {
    float **A, **B, **C;
    int i, j, t;
    double start, end;
    double base_time = 0.0;

    // Allocate memory
    A = (float **)malloc(size * sizeof(float *));
    B = (float **)malloc(size * sizeof(float *));
    C = (float **)malloc(size * sizeof(float *));
    for (i = 0; i < size; i++) {
        A[i] = (float *)malloc(size * sizeof(float));
        B[i] = (float *)malloc(size * sizeof(float));
        C[i] = (float *)malloc(size * sizeof(float));
    }

    // Initialize matrices
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
        }

    printf("\nMatrix Size: %d x %d\n", size, size);
    printf("Threads\tTime (s)\tSpeedup\n");

    for (t = 1; t <= 8; t *= 2) {
        omp_set_num_threads(t);
        start = omp_get_wtime();

        // Parallel matrix addition
        #pragma omp parallel for collapse(2) schedule(static)
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                C[i][j] = A[i][j] + B[i][j];

        end = omp_get_wtime();
        double time_taken = end - start;

        if (t == 1)
            base_time = time_taken;

        double speedup = base_time / time_taken;
        printf("%d\t%.6f\t%.2f\n", t, time_taken, speedup);
    }

    // Free memory
    for (i = 0; i < size; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};

    printf("2D Matrix Addition using OpenMP (Speedup Analysis)\n");

    for (int i = 0; i < 5; i++) {
        matrix_addition(sizes[i]);
    }

    return 0;
}

