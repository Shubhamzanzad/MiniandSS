/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to change the exiting message queue permission. (use msqid_ds structure)
File name : 28.c
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

    struct msqid_ds buf;

    // Get current message queue information
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl (IPC_STAT)");
        exit(EXIT_FAILURE);
    }

    // Modify permissions (set read, write, and execute permissions for owner and group)
    buf.msg_perm.mode = 0666; // Change the permission as needed

    // Set the new permissions
    if (msgctl(msgid, IPC_SET, &buf) == -1) {
        perror("msgctl (IPC_SET)");
        exit(EXIT_FAILURE);
    }

    printf("Message queue permissions changed.\n");

    return 0;
}
