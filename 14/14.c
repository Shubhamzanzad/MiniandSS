/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
File name : 14.c
Date : 15/09/2023
==================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    /*pid_t pid = fork();
    if (pid == -1) {
        perror("fork not created!!!!!!!!");
        exit(EXIT_FAILURE);
    }
*/
    if (!fork()) {
        // reader
        close(pipefd[1]); // Close write end of the pipe

        char buffer[100];
        ssize_t num_bytes_read = read(pipefd[0], buffer, sizeof(buffer));

        if (num_bytes_read == -1) {
            perror("you didn't enetered anything!!!!");
            exit(EXIT_FAILURE);
        }

        buffer[num_bytes_read] = '\0';
        printf("Child process received\n%s\n", buffer);
        close(pipefd[0]);
    } 
        else {
        // writer
        close(pipefd[0]); // Close read end of the pipe

        const char *message = "Hello, how are you??";
        printf("Parent process entered a message!!\n");
        ssize_t num_bytes_written = write(pipefd[1], message, 21);

        if (num_bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]); // Close the write end of the pipe in the parent process
    }
    return 0;
}
