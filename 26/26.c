#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    // Replace with the appropriate key (obtained from a message queue creation)
    key = ftok("/tmp", 'B');

    // Obtain the message queue ID
    msgid = msgget(key, 0666|IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Prompt user for a message to send
    printf("Enter message to send: ");
    fgets(msg.msg_text, 100, stdin);
    msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0';  // Remove trailing newline
    msg.msg_type = 1;

    // Send the message
    if (msgsnd(msgid, &msg, sizeof(struct msg_buffer), IPC_NOWAIT) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Message sent to the queue.\n");
    return 0;
}
