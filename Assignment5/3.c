#include <stdio.h>
#include <omp.h>

#define N 500

int main() {
    static double A[N][N], x[N], y[N];
    int i, j;

    // Initialize matrix and vector
    for (i = 0; i < N; i++) {
        x[i] = i * 1.0;
        y[i] = 0.0;
        for (j = 0; j < N; j++)
            A[i][j] = i + j;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for private(j) shared(A, x, y)
    for (i = 0; i < N; i++) {
        double sum = 0.0;
        for (j = 0; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }

    double end = omp_get_wtime();
    printf("Matrix-Vector Multiplication done in %f seconds\n", end - start);
    return 0;
}
