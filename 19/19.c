/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Create a FIFO file by
a. mknod command
b. mkfifo command
c. use strace command to find out, which command (mknod or mkfifo) is better.
c. mknod system call
d. mkfifo library function
File name : 19.c
Date : 12/10/2023
==================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<errno.h>
#include<stdio.h>
int main() {
    if (mknod("mknod_fifo_prog", S_IFIFO | 0666, 0) == 0) {
        printf("FIFO file created using mknod system call.\n");
    } 
    else {
        if(errno != EEXIST) {
            perror("mknod");
            return 1;
        }
    }

    if (mkfifo("mkfifo_fifo_prog", 0666) == 0) {
        printf("FIFO file created using mkfifo library function.\n");
    } 
    else {
        if(errno !=  EEXIST) {
            perror("mkfifo");
            return 2;
        }
    }
    return 0;
}
