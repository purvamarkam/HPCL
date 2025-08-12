#include <stdio.h>
#include <omp.h>
#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0; // number of items in buffer

void produce_item(int item, int tid) {
    printf("Producer Thread %d: Produced %d\n", tid, item);
}

void consume_item(int item, int tid) {
    printf("Consumer Thread %d: Consumed %d\n", tid, item);
}

int main() {
    int i;
    
    #pragma omp parallel sections shared(buffer, count) private(i)
    {
        // Producer section
        #pragma omp section
        {
            int tid = omp_get_thread_num();
            for (i = 1; i <= 10; ) {
                #pragma omp critical
                {
                    if (count < BUFFER_SIZE) {
                        buffer[count] = i;
                        count++;
                        produce_item(i, tid);
                        i++; // move to next item only when produced
                    }
                }
                #pragma omp flush
            }
        }

        // Consumer section
        #pragma omp section
        {
            int tid = omp_get_thread_num();
            for (i = 1; i <= 10; ) {
                #pragma omp critical
                {
                    if (count > 0) {
                        int item = buffer[count - 1];
                        count--;
                        consume_item(item, tid);
                        i++; // consume only when available
                    }
                }
                #pragma omp flush
            }
        }
    }

    return 0;
}
