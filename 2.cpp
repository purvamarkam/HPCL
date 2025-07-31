#include <stdio.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter number of threads: ");
    scanf("%d", &n);

    // Sequential output
    printf("\nSequential Hello:\n");
    for (int i = 0; i < n; i++) {
        printf("Hello, World from thread %d\n", i);
    }

    // Parallel output
    printf("\nParallel Hello:\n");
    omp_set_num_threads(n);
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("Hello, World from thread %d\n", ID);
    }

    return 0;
}
