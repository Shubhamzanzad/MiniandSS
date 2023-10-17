/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to execute ls -l | wc.
a. use dup
b. use dup2
c. use fcntl
File name : 17b.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    if (!fork()) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execlp("wc", "wc", (char *)NULL);
    } 
    else {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);
    }
    return 0;
}

