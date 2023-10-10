#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main() {
    sem_t bin, count;

    if (sem_init(&bin, 0, 1) == -1) {
        perror("sem_init (binary semaphore)");
        return 1;
    }

    // Initialize counting semaphore with an initial value of 5 (counting)
    if (sem_init(&count, 0, 12) == -1) {
        perror("sem_init (counting)");
        return 2;
    }

    printf("Binary semaphore created and initialized with value 1.\n");
    printf("Counting semaphore created and initialized with value 12.\n");
    return 0;
}
