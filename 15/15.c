/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a simple program to send some data from parent to the child process.
File name : 15.c
Date : 4/10/2023
==================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int fd[2];
    char buffer[100];
    if (pipe(fd) == -1) {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }

    if (!fork()) {
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Child process received: %s\n", buffer);
        close(fd[0]);
    } 
    else {
        close(fd[0]);
        printf("Enter msg for child process :");
        scanf("%[^\n]", buffer);
        write(fd[1], buffer, sizeof(buffer));
        close(fd[1]);
    }
    return 0;
}

