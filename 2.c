#include <stdio.h>
#include <omp.h>

int main() {
    long num_steps = 100000000;
    double step = 1.0 / num_steps;
    double sum = 0.0;
    int num_threads;

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);
    omp_set_num_threads(num_threads);

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        double x, local_sum = 0.0;
        #pragma omp for
        for (int i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    double pi = step * sum;
    double end = omp_get_wtime();

    printf("Estimated Pi = %.15f\n", pi);
    printf("Time taken with %d threads: %f seconds\n", num_threads, end - start);

    return 0;
}
