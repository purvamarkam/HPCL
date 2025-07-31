#include <stdio.h>
#include <omp.h>
#define SIZE 20  

int main() {
    int i;
    float scalar = 5.0, A[SIZE], B[SIZE];

    // Initialize array A
    for (i = 0; i < SIZE; i++)
        A[i] = i * 1.0;

    double start = omp_get_wtime();

    // Parallel vector-scalar addition
    #pragma omp parallel for
    for (i = 0; i < SIZE; i++) {
        int tid = omp_get_thread_num();
        B[i] = A[i] + scalar;
        printf("Thread %d processed index %d\n", tid, i);
    }

    double end = omp_get_wtime();

    printf("Time taken: %f seconds\n", end - start);
    return 0;
}
