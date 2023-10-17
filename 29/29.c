/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to remove the message queue.
File name : 29.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main() {
    key_t key;
    int msgid;

    // Replace with the appropriate key (obtained from a message queue creation)
    key = ftok("/tmp", 'B');

    // Obtain the message queue ID
    msgid = msgget(key, 0);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl (IPC_RMID)");
        exit(EXIT_FAILURE);
    }

    printf("Message queue removed.\n");

    return 0;
}
