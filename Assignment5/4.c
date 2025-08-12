#include <stdio.h>
#include <omp.h>

#define N 1000000

int main() {
    static int arr[N], prefix[N];
    int i;

    // Initialize array
    for (i = 0; i < N; i++)
        arr[i] = 1; 

    double start = omp_get_wtime();

    prefix[0] = arr[0];
    #pragma omp parallel
    {
        #pragma omp for
        for (i = 1; i < N; i++) {
            prefix[i] = prefix[i - 1] + arr[i]; 
        }
    }

    double end = omp_get_wtime();
    printf("Prefix sum done in %f seconds\n", end - start);
    return 0;
}
