#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MAX_MSG_SIZE 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    // Replace with the appropriate key (obtained from a message queue creation)
    key = ftok("/tmp", 'B');

    // Obtain the message queue ID
    msgid = msgget(key, 0);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Receive message with 0 as a flag (blocking)
    if (msgrcv(msgid, &msg, sizeof(struct msg_buffer), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Received message (blocking): %s\n", msg.msg_text);

    // Receive message with IPC_NOWAIT as a flag (non-blocking)
    int ret = msgrcv(msgid, &msg, sizeof(struct msg_buffer), 1, IPC_NOWAIT);
    if (ret == -1) {
        if (errno == ENOMSG) {
            printf("No message available (non-blocking).\n");
        } 
        else {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        printf("Received message (non-blocking): %s\n", msg.msg_text);
    }

    return 0;
}