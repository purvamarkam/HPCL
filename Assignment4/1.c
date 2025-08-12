#include <stdio.h>
#include <omp.h>

long long fib(int n) {
    if (n < 2) return n;

    long long x, y;

    #pragma omp task shared(x) firstprivate(n)
    x = fib(n - 1);

    #pragma omp task shared(y) firstprivate(n)
    y = fib(n - 2);

    #pragma omp taskwait

    return x + y;
}

int main() {
    int n = 10;
    long long result;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        // Ensure only one thread starts the recursion
        #pragma omp single
        result = fib(n);
    }

    double end = omp_get_wtime();

    printf("Fibonacci(%d) = %lld\n", n, result);
    printf("Time taken: %f seconds\n", end - start);

    return 0;
}
