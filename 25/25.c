#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include<time.h>
void print_message_queue_info(int msgid) {
    struct msqid_ds buf;

    // Get message queue information
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Access permissions
    printf("Access permissions: %#o\n", buf.msg_perm.mode);

    // UID and GID
    printf("UID: %d, GID: %d\n", buf.msg_perm.uid, buf.msg_perm.gid);

    // Time of last message sent and received
    printf("Time of last message sent: %s", ctime(&buf.msg_stime));
    printf("Time of last message received: %s", ctime(&buf.msg_rtime));

    // Time of last change in the message queue
    printf("Time of last change: %s", ctime(&buf.msg_ctime));

    // Size of the queue
    printf("Size of the queue: %lu\n", buf.msg_qbytes);

    // Number of messages in the queue
    printf("Number of messages in the queue: %lu\n", buf.msg_qnum);

    // Maximum number of bytes allowed
    printf("Maximum number of bytes allowed: %lu\n", buf.msg_qbytes);

    // PID of the msgsnd and msgrcv
    printf("PID of msgsnd: %d\n", buf.msg_lspid);
    printf("PID of msgrcv: %d\n", buf.msg_lrpid);
}

int main() {
    key_t key;
    int msgid;

    // Replace with the appropriate key (obtained from a message queue creation)
    key = ftok(".", 'B');

    // Obtain the message queue ID
    msgid = msgget(key, 0666|IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    // Print message queue information
    print_message_queue_info(msgid);

    return 0;
}