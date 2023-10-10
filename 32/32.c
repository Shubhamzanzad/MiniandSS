#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include <unistd.h>

// Global variables for shared resources
int ticketNumber = 0;
char* sharedMemory;
sem_t ticketSemaphore, memorySemaphore, resourceSemaphore;

// Function to generate a ticket number
void* generateTicket(void* arg) {
    int threadID = *((int*)arg);
    int ticket;

    // Wait for access to the critical section
    sem_wait(&ticketSemaphore);

    // Generate a ticket number
    ticket = ticketNumber++;
    printf("Thread %d generated ticket: %d\n", threadID, ticket);

    // Release access to the critical section
    sem_post(&ticketSemaphore);

    pthread_exit(NULL);
}

// Function to write to shared memory
void* writeToSharedMemory(void* arg) {
    int threadID = *((int*)arg);
    char message[100];

    // Wait for access to the shared memory
    sem_wait(&memorySemaphore);

    // Write to shared memory
    sprintf(message, "Thread %d wrote to shared memory", threadID);
    strcpy(sharedMemory, message);
    printf("%s\n", message);

    // Release access to the shared memory
    sem_post(&memorySemaphore);

    pthread_exit(NULL);
}

// Function to access pseudo resources
void* accessResource(void* arg) {
    int threadID = *((int*)arg);

    // Wait for access to a resource
    sem_wait(&resourceSemaphore);

    // Access the resource
    printf("Thread %d is accessing a resource.\n", threadID);

    // Release access to the resource
    sem_post(&resourceSemaphore);

    pthread_exit(NULL);
}

int main() {
    int i, numThreads = 5; // Number of threads

    // Initialize semaphores
    sem_init(&ticketSemaphore, 0, 1);   // Binary semaphore for ticket generation
    sem_init(&memorySemaphore, 0, 1);   // Binary semaphore for shared memory access
    sem_init(&resourceSemaphore, 0, 2); // Counting semaphore for two resources

    // Create threads for different tasks
    pthread_t ticketThreads[numThreads];
    pthread_t memoryThreads[numThreads];
    pthread_t resourceThreads[numThreads];

    // Create shared memory segment
    key_t shmKey = ftok("/tmp", 'S');
    int shmid = shmget(shmKey, 100, IPC_CREAT | 0666);
    sharedMemory = (char*)shmat(shmid, NULL, 0);

    // Create threads and perform tasks
    for (i = 0; i < numThreads; i++) {
        int threadID = i + 1;
        pthread_create(&ticketThreads[i], NULL, generateTicket, &threadID);
        pthread_create(&memoryThreads[i], NULL, writeToSharedMemory, &threadID);
        pthread_create(&resourceThreads[i], NULL, accessResource, &threadID);
    }

    // Wait for threads to finish
    for (i = 0; i < numThreads; i++) {
        pthread_join(ticketThreads[i], NULL);
        pthread_join(memoryThreads[i], NULL);
        pthread_join(resourceThreads[i], NULL);
    }

    // Detach and remove shared memory
    shmdt(sharedMemory);
    shmctl(shmid, IPC_RMID, NULL);

    // Destroy semaphores
    sem_destroy(&ticketSemaphore);
    sem_destroy(&memorySemaphore);
    sem_destroy(&resourceSemaphore);
    printf("Destroyed all three semaphores.\n");
    return 0;
}