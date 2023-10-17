/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
File name : 22b.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
int main() {
    int fd = open("myfifo", O_WRONLY);
    char buff[50];
    printf("Enter a msg\n");
    scanf("%[^\n]", buff);
    write(fd, &buff, sizeof(buff));
    return 0;
}