/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory
File name : 30.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    key_t key;
    int shmid;
    char *shmaddr;
    key = ftok("/tmp", 'S');

    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        return 2;
    }

    strcpy(shmaddr, "Hello, shared memory!");
    printf("Data in shared memory: %s\n", shmaddr);
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        return 3;
    }
    shmaddr = shmat(shmid, NULL, SHM_RDONLY);
    if (shmaddr == (char *)-1) {
        perror("shmat (read-only)");
        return 4;
    }
    printf("After this it will cause a segmentation fault because we trying to write in read-only file\n");
    printf("To avoid segmentation fault just comment the next line\n");
    strcpy(shmaddr, "Trying to overwrite");
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        return 5;
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (IPC_RMID)");
        return 6;
    }

    printf("Shared memory removed.\n");

    return 0;
}
