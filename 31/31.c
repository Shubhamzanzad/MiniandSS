/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to implement semaphore to protect any critical section.
a. rewrite the ticket number creation program using semaphore
b. protect shared memory from concurrent write access
c. protect multiple pseudo resources ( may be two) using counting semaphore
d. remove the created semaphore
File name : 31.c
Date : 12/10/2023
==================================================
*/
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
