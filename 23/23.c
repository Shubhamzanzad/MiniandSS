/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to print the maximum number of files can be opened within a process and
size of a pipe (circular buffer).
File name : 23.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    // Get the maximum number of files that can be opened
    long max_files = sysconf(_SC_OPEN_MAX);
    if (max_files == -1)
        perror("Failed to get maximum number of files");
    else
        printf("Maximum number of files that can be opened: %ld\n", max_files);

    // Get the size of a pipe's circular buffer
    long pipe_buffer_size = sysconf(_PC_PIPE_BUF);
    if (pipe_buffer_size == -1)
        perror("Failed to get pipe buffer size");
    else
        printf("Size of a pipe's circular buffer: %ld bytes\n", pipe_buffer_size);

    return 0;
}
