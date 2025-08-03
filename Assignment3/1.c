// Problem 1: Scalar (Dot) Product using OpenMP reduction
#include <stdio.h>
#include <omp.h>

#define SIZE 1000

int main() {
    int i;
    float a[SIZE], b[SIZE], result = 0.0;

    // Initialize arrays
    for (i = 0; i < SIZE; i++) {
        a[i] = i * 1.0;
        b[i] = (SIZE - i) * 1.0;
    }

    // Parallel dot product using reduction
    #pragma omp parallel for reduction(+:result)
    for (i = 0; i < SIZE; i++) {
        result += a[i] * b[i];
    }

    printf("Scalar (Dot) Product = %.2f\n", result);
    return 0;
}
