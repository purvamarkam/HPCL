#include <stdio.h>
#include <omp.h>

#define N 1000

int main() {
    static double A[N][N];
    double scalar = 5.0;
    int i, j;

    // Initialize matrix
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = i + j;

    double start = omp_get_wtime();

    #pragma omp parallel for private(j) shared(A)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] *= scalar;
        }
    }

    double end = omp_get_wtime();
    printf("Matrix-Scalar Multiplication done in %f seconds\n", end - start);
    return 0;
}
