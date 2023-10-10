#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define KEY_PATH "/tmp"  // Path to generate a key

int main() {
    key_t key;
    int msgid;

    // Generate a key using ftok
    if ((key = ftok(KEY_PATH, 'B')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Message queue created with key: %x and ID: %d\n", key, msgid);

    return 0;
}
