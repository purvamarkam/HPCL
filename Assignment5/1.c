#include <stdio.h>
#include <omp.h>

#define N 500

int main() {
    static double A[N][N], B[N][N], C[N][N];
    int i, j, k;

    // Initialize matrices
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0.0;
        }

    double start = omp_get_wtime();

    #pragma omp parallel for private(j, k) shared(A, B, C)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            double sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    double end = omp_get_wtime();
    printf("Matrix-Matrix Multiplication done in %f seconds\n", end - start);
    return 0;
}
